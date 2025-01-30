#include "module.hpp"
#include <format>
#include <chrono>
#include <string>
using namespace std::string_literals;
static const string tname = format("{}.{}", module_name, "span");
template<> const char* span_t::type_name(){return tname.c_str();}

span* to_span(lua_State* L, int idx) {
    return &span_t::check(L, idx);
}
static std::string default_format(span amount) {
    auto h = duration_cast<hours>(amount);
    amount -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(amount);
    amount -= m;
    auto s = std::chrono::duration_cast<std::chrono::seconds>(amount);
    amount -= s;
    auto ns = amount;
    std::string result = std::format("{:02}:{:02}:{:02}.{:09}", h.count(),
        m.count(), s.count(), ns.count());
    return result;
}
static const span_t::registry namecall = {
    {"format", [](lua_State* L, span& d) -> int {
        const std::string fmt = "{:"s + luaL_checkstring(L, 2) + "}"s;
        lua_pushstring(L, std::vformat(fmt, std::make_format_args(d)).c_str());
        return 1;
    }},
    {"type", [](lua_State* L, span& d) -> int {
        lua_pushstring(L, tname.c_str());
        return 1;
    }},
};

static const span_t::registry indices = {
    {"total_seconds", [](lua_State* L, span& self) -> int {
        lua_pushnumber(L, duration<double>(self).count());
        return 1;
    }},{"total_microseconds", [](lua_State* L, span& self) -> int {
        lua_pushnumber(L, duration<double, std::micro>(self).count());
        return 1;
    }},{"total_nanoseconds", [](lua_State* L, span& self) -> int {
        lua_pushnumber(L, duration<double, std::nano>(self).count());
        return 1;
    }},{"total_minutes", [](lua_State* L, span& self) -> int {
        lua_pushnumber(L, duration<double, std::ratio<60>>(self).count());
        return 1;
    }},{"total_hours", [](lua_State* L, span& self) -> int {
        lua_pushnumber(L, duration<double, std::ratio<60*60>>(self).count());
        return 1;
    }},
};

static int sub(lua_State* L) {
    if (point_t::is(L, 1)) {
        auto& p = point_t::check(L, 1);
        new_point(L, point(
            p.get_time_zone(),
            p.get_sys_time() - duration_cast<milliseconds>(span_t::check(L, 2))
        ));
        return 1;
        return 1;
    } else if (nanopoint_t::is(L, 1)) {
        new_nanopoint(L, nanopoint_t::check(L, 1) - span_t::check(L, 2));
        return 1;
    } else if (span_t::is(L, 1) and span_t::is(L, 2)) {
        new_span(L, *to_span(L, 1) - *to_span(L, 2));
        return 1;
    }
    luaL_errorL(L, "unknown arithmetic operation");
}
static int add(lua_State* L) {
    if (point_t::is(L, 1)) {
        auto& p = point_t::check(L, 1);
        new_point(L, point(
            p.get_time_zone(),
            p.get_sys_time() + duration_cast<milliseconds>(span_t::check(L, 2))
        ));
        return 1;
    } else if (nanopoint_t::is(L, 1)) {
        new_nanopoint(L, nanopoint_t::check(L, 1) + span_t::check(L, 2));
        return 1;
    } else if (span_t::is(L, 1) and span_t::is(L, 2)) {
        new_span(L, span_t::check(L, 1) + span_t::check(L, 2));
        return 1;
    }
    luaL_errorL(L, "unknown arithmetic operation");
}

static int tostring(lua_State* L) {
    span* p = to_span(L, 1);
    lua_pushstring(L, default_format(*p).c_str());
    return 1;
}
span& new_span(lua_State* L, const span& v) {
    if (not span_t::initialized(L)) {
        const luaL_Reg meta[] = {
            {"__tostring", tostring},
            {"__sub", sub},
            {"__add", add},
            {nullptr, nullptr}
        };
        span_t::init(L, {
            .index = indices,
            .namecall = namecall,
            .meta = meta,
        });
    }
    return span_t::create(L, v);
}

