#pragma once
#include <dluau.h>
#include <string>
#include <unordered_map>
#include <boost/container/flat_map.hpp>
#include <cassert>

template<class T>
class tagged_type {
public:
    static const char* type_name();
    inline static int tag{};
    using action = int(*)(lua_State*, T&);
    using registry = boost::container::flat_map<std::string, action>;
    static bool is_type(lua_State* L, int idx) {
        return lua_userdatatag(L, idx) == tag;
    }
    static T& check_udata(lua_State* L, int idx) {
        if (not is_type(L, idx)) luaL_typeerrorL(L, idx, type_name());
        return *static_cast<T*>(lua_touserdatatagged(L, idx, tag));
    }
    static T& new_udata(lua_State* L, const T& v) {
        T* p = static_cast<T*>(lua_newuserdatatagged(L, sizeof(T), tag));
        luaL_getmetatable(L, type_name());
        lua_setmetatable(L, -2);
        new (p) T{v};
        return *p;
    }
    static T& new_udata(lua_State* L, T&& v) {
        T* p = static_cast<T*>(lua_newuserdatatagged(L, sizeof(T), tag));
        luaL_getmetatable(L, type_name());
        lua_setmetatable(L, -2);
        new (p) T{std::move(v)};
        return *p;
    }
    static bool initialized(lua_State* L) {
        if (not initialized_) {
            luaL_getmetatable(L, type_name());
            if (not lua_isnil(L, -1)
                and dluau_istyperegistered(type_name())) {
                initialized_ = true;
                tag = dluau_gettagfromtname(type_name());
            }
            lua_pop(L, 1);
        }
        return initialized_;
    }
    struct init_info {
        registry index{};
        registry newindex{};
        registry namecall{};
        const luaL_Reg* meta{};
    };
    static void init(lua_State* L, init_info info) {
        if (initialized_) return;
        initialized_ = true;
        if (luaL_newmetatable(L, type_name())) {
            const luaL_Reg meta[] = {
                {"__index", metamethod_index},
                {"__newindex", metamethod_newindex},
                {"__namecall", metamethod_namecall},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, meta);
            if (info.meta) luaL_register(L, nullptr, info.meta);
            lua_pushstring(L, type_name());
            lua_setfield(L, -2, "__type");
            init_namecalls(L, info.namecall);
            index_ = std::move(info.index);
            newindex_ = std::move(info.newindex);
        }
        lua_pop(L, 1);
        if (not dluau_istyperegistered(type_name())) {
            tag = dluau_registertypetagged(type_name());
            lua_setuserdatadtor(L, tag, [](lua_State* L, void* data) {
                static_cast<T*>(data)->~T();
            });
        }
    }
private:
    static int metamethod_namecall(lua_State* L) {
        auto& self = check_udata(L, 1);
        int atom;
        lua_namecallatom(L, &atom);
        auto found_it = namecall_.find(atom);
        if (found_it == namecall_.end()) luaL_errorL(L, "invalid namecall");
        return found_it->second(L, self);
    }
    static int metamethod_index(lua_State* L) {
        T& self = check_udata(L, 1);
        const std::string key = luaL_checkstring(L, 2); 
        auto found_it = index_.find(key);
        if (found_it == index_.end()) luaL_errorL(L, "invalid index");
        return found_it->second(L, self);
    }
    static int metamethod_newindex(lua_State* L) {
        T& self = check_udata(L, 1);
        const std::string key = luaL_checkstring(L, 2); 
        auto found_it = newindex_.find(key);
        if (found_it == newindex_.end()) luaL_errorL(L, "invalid index");
        return found_it->second(L, self);
    }
    static void init_namecalls(lua_State* L, const registry& namecalls) {
        for (const auto& [key, call] : namecalls) {
            lua_pushlstring(L, key.data(), key.size());
            int atom;
            lua_tostringatom(L, -1, &atom);
            lua_pop(L, 1);
            namecall_.emplace(atom, call);
        }
    }
    inline static bool initialized_ = false;
    inline static boost::container::flat_map<int, action> namecall_;
    inline static registry index_;
    inline static registry newindex_;
};
