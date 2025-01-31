#pragma once
#include <chrono>
#include <format>
#include "type.hpp"
using namespace std::chrono;
using span = nanoseconds;
using point = zoned_time<milliseconds>;
using nanopoint = time_point<steady_clock, span>;
using date = year_month_day;
point& new_point(lua_State* L, const point& v);
span& new_span(lua_State* L, const span& v);
nanopoint& new_nanopoint(lua_State* L, const nanopoint& v);

using span_t = type<span>;
using point_t = type<point>;
using nanopoint_t = type<nanopoint>;
using std::format, std::string;
constexpr const char* module_name = "time";

