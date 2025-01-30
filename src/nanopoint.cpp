#include "module.hpp"
#include <format>
#include <chrono>
#include "type.hpp"
#include <string>
using namespace std::string_literals;
using nanopoint_t = type<nanopoint>;
static const std::string tname = format("{}.{}", module_name, "nanopoint");
template<> const char* nanopoint_t::type_name(){return tname.c_str();}

static const nanopoint_t::registry index = {
    {"hour", [](lua_State* L, nanopoint& tp) -> int {
        const auto midnight = tp - floor<days>(tp);
        const auto hrs = duration_cast<hours>(midnight);
        lua_pushinteger(L, hrs.count());
        return 1;
    }},
    {"minute", [](lua_State* L, nanopoint& tp) -> int {
        const auto midnight = tp - floor<days>(tp);
        const auto hrs = duration_cast<hours>(midnight);
        const auto min = duration_cast<minutes>(midnight - hrs);
        lua_pushinteger(L, min.count());
        return 1;
    }},
    {"second", [](lua_State* L, nanopoint& tp) -> int {
        const auto midnight = tp - floor<days>(tp);
        const auto hrs = duration_cast<hours>(midnight);
        const auto min = duration_cast<minutes>(midnight - hrs);
        const auto sec = duration_cast<seconds>(midnight - hrs - min);
        lua_pushinteger(L, sec.count());
        return 1;
    }},
    {"millisecond", [](lua_State* L, nanopoint& tp) -> int {
        const auto midnight = tp - floor<days>(tp);
        const auto hrs = duration_cast<hours>(midnight);
        const auto min = duration_cast<minutes>(midnight - hrs);
        const auto sec = duration_cast<seconds>(midnight - hrs - min);
        const auto milli = duration_cast<milliseconds>(midnight - hrs - min - sec);
        lua_pushinteger(L, milli.count());
        return 1;
    }},
    {"microsecond", [](lua_State* L, nanopoint& tp) -> int {
        const auto midnight = tp - floor<days>(tp);
        const auto hrs = duration_cast<hours>(midnight);
        const auto min = duration_cast<minutes>(midnight - hrs);
        const auto sec = duration_cast<seconds>(midnight - hrs - min);
        const auto milli = duration_cast<milliseconds>(
            midnight - hrs - min - sec);
        const auto micro = duration_cast<microseconds>(
            midnight - hrs - min - sec - milli);
        lua_pushinteger(L, micro.count());
        return 1;
    }},
    {"nanosecond", [](lua_State* L, nanopoint& tp) -> int {
        const auto midnight = tp - floor<days>(tp);
        const auto hrs = duration_cast<hours>(midnight);
        const auto min = duration_cast<minutes>(midnight - hrs);
        const auto sec = duration_cast<seconds>(midnight - hrs - min);
        const auto milli = duration_cast<milliseconds>(
            midnight - hrs - min - sec);
        const auto micro = duration_cast<microseconds>(
            midnight - hrs - min - sec - milli);
        const auto nano = duration_cast<nanoseconds>(
            midnight - hrs - min - sec - milli);
        lua_pushinteger(L, nano.count());
        return 1;
    }},
};
static int sub(lua_State* L) {
    new_span(L, nanopoint_t::check(L, 1) - nanopoint_t::check(L, 2));
    return 1;
}

static int tostring(lua_State* L) {
    nanopoint& tp = nanopoint_t::check(L, 1);
    lua_pushstring(L, std::format("{}", tp.time_since_epoch()).c_str());
    return 1;
}

nanopoint& new_nanopoint(lua_State* L, const nanopoint& v) {
    if (not nanopoint_t::initialized(L)) {
        const luaL_Reg meta[] = {
            {"__tostring", tostring},
            {"__sub", sub},
            {nullptr, nullptr}
        };
        nanopoint_t::init(L, {
            .index = index,
            .meta = meta,
        });
    }
    return nanopoint_t::create(L, v);
}
