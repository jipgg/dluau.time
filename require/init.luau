type unit_union = 'ns'|'us'|'s'|'min'|'h'|'d'|'mo'|'y' 
--- Represents a span of time, expressed in various units.
--- A `span` is a userdata type that supports arithmetic operations and conversions between different time units.
export type span = typeof(setmetatable({} :: _span, {} :: __span))
type _span = {
    --- The span represented in nanoseconds.
    total_nanoseconds: number,
    --- The span represented in microseconds.
    total_microseconds: number,
    --- The span represented in milliseconds.
    total_milliseconds: number,
    --- The span represented in seconds.
    total_seconds: number,
    --- The span represented in minutes.
    total_minutes: number,
    --- The span represented in hours.
    total_hours: number,
    --- Formats the `span` as a string according to the provided format string.
    --- Uses standard C++ formatting specifiers for std::chrono's `nanoseconds` duration type.
    --- **Format specifiers**
    --- - `%H`: Hours as a two-digit number (e.g., `01` for 1 hour).
    --- - `%M`: Minutes as a two-digit number (e.g., `59`).
    --- - `%S`: Seconds with optional fractional seconds (e.g., `59.123456`).
    --- - `%T`: Full time representation, including hours, minutes, and seconds (e.g., `01:59:59.123`).
    --- - `%R`: Time without seconds (e.g., `01:59`).
    --- - `%p`: AM/PM designation for 12-hour clocks (e.g., `AM` or `PM`).
    --- **Example usage**
    --- ```lua
    --- local dt = require('@std/date_time')
    --- local duration = dt.hours(10) + dt.seconds(20)
    --- print(duration:format'%T')
    --- ```
    --- @param fmt string The format string to use for formatting.
    --- @return string The formatted string representation of the duration.
    format: ((self: span, fmt: string ) -> string)
        & ((self: span, presets:'%H'|'%M'|'%S'|'%T'|'%S'|'%R'|'%p') -> string),
}
type __span = {
    __sub: (self: span, other: span) -> span,
    __add: (self: span, other: span) -> span,
    __type: "time.span"
}
--- Represents a specific point in time, defined in terms of calendar date and time.
--- A `point` is a userdata type that supports formatting and arithmetic with durations.
export type point = typeof(setmetatable({} :: _point, {} :: __point))
type _point = {
    --- the time zone name
    time_zone: string,
    --- time zone abbreviation
    zone_abbreviation: string,
    --- returns a copy of the `point` with the time converted to the specified time zone
    change_zone: (self: point, time_zone: string) -> point,
    --- The year of the time point (e.g., 2025).
    year: number,
    --- The month of the time point (1-12).
    month: number,
    --- The day of the time point (1-31, depending on the month).
    day: number,
    --- The hour of the time point (0-23).
    hour: number,
    --- The minute of the time point (0-59).
    minute: number,
    --- The second of the time point (0-59).
    second: number,
    --- The millisecond of the time point (0-59).
    millisecond: number,
    --- Formats the `point` as a string according to the provided format string.
    --- Uses standard C++ formatting for std::chrono's `zoned_time<milliseconds>`.
    --- **Format specifiers**
    --- - `%Y`: Full year (e.g., `2025`).
    --- - `%C`: Century (year divided by 100, truncated, e.g., `20` for 2025).
    --- - `%y`: Last two digits of the year (e.g., `25` for 2025).
    --- - `%m`: Month as a two-digit number (e.g., `01` for January).
    --- - `%B`: Full month name (e.g., `January`).
    --- - `%b`: Abbreviated month name (e.g., `Jan`).
    --- - `%d`: Day of the month as a two-digit number (e.g., `05`).
    --- - `%e`: Day of the month, single-digit space-padded (e.g., ` 5` for 5th day).
    --- - `%H`: Hour in 24-hour format (e.g., `14` for 2 PM).
    --- - `%I`: Hour in 12-hour format (e.g., `02` for 2 PM).
    --- - `%M`: Minute as a two-digit number (e.g., `35`).
    --- - `%S`: Seconds with optional fractional seconds (e.g., `45.6789`).
    --- - `%p`: AM/PM designation (e.g., `PM`).
    --- - `%z`: UTC offset in `+HHMM` or `-HHMM` format (e.g., `+0530`).
    --- - `%Z`: Time zone abbreviation (e.g., `UTC`).
    --- - `%j`: Day of the year (e.g., `005` for January 5).
    --- - `%U`: Week number with Sunday as the first day (e.g., `01`).
    --- - `%W`: Week number with Monday as the first day (e.g., `01`).
    --- - `%c`: Locale-specific date and time (e.g., `Tue Jan 5 14:35:45 2025`).
    --- - `%x`: Locale-specific date representation (e.g., `01/05/2025`).
    --- - `%X`: Locale-specific time representation (e.g., `14:35:45`).
    --- - `%%`: Literal `%` character.
    --- @param fmt string The format string to use for formatting.
    --- @return string The formatted string representation of the time point.
    format: ((self: point, fmt: string) -> string)
        & ((self: point, presets:'%Y'|'%C'|'%y'|'%m'|
            '%B'|'%b'|'%d'|'%e'|'%H'|'%I'|'%M'|'%S'|'%p'|
            '%z'|'%Z'|'%j'|'%U'|'%W'|'%c'|'%x'|'%X'|'%%') -> string),
}
type __point = {
    __sub: ((self: point, other: point) -> span)
        & ((self: point, other: span) -> point),
    __add: (self: point, other: span) -> point,
    __type: "time.point"
}
export type nanopoint = typeof(setmetatable({} :: _nanopoint, {} :: __nanopoint))
type _nanopoint = {
    hour: number,
    --- The minute of the time point (0-59).
    minute: number,
    --- The second of the time point (0-59).
    second: number,
    --- The millisecond of the time point.
    millisecond: number,
    --- The microsecond of the time point.
    microsecond: number,
    --- The nanosecond of the time point.
    nanosecond: number,
}
type __nanopoint = {
    __sub: ((self: nanopoint, other: nanopoint) -> span)
        & ((self: nanopoint, other: span) -> nanopoint),
    __add: (self: nanopoint, other: span) -> nanopoint,
    __type: "time.nanopoint"
}
--- Provides a standard set of utilities for working with time.
--- Internally implemented using `std::chrono` from C++20, this module 
--- offers functionality to retrieve the current time, perform arithmetic with durations, and 
--- work with high-precision clocks. 
export type time = {
    --- local time zone
    current_zone: string,
    --- Retrieves the current time as a `point`.
    --- If no time zone is specified it uses the local time zone.
    now: (timezone: string?) -> point,
    --- High precision clock for benchmarking.
    --- std::chrono::steady_clock::now() internallly.
    nano_now: () -> nanopoint,
    --- Retrieves the utc time as a `point`.
    utc_now: () -> point,
    --- Constructs a `point` from the given date and time.
    --- @param year `number`: The year (e.g., 2025).
    --- @param month `number`: The month (1-12).
    --- @param day `number`: The day (1-31).
    --- @param hour `number`: The hour (0-23).
    --- @param minute `number`: The minute (0-59).
    --- @param second `number`: The second (0-59).
    --- @return `point`: The constructed time point.
    from_datetime: ((year: number, month: number, day: number,
        hour: number, minute: number, second: number) -> point),
    --- Constructs a `point` from the given date.
    --- @param year `number`: The year (e.g., 2025).
    --- @param month `number`: The month (1-12).
    --- @param day `number`: The day (1-31).
    --- @return `point`: The constructed time point.
    from_date: (year: number, month: number, day: number) -> point,
    --- Creates a `span` representing the specified number of seconds.
    --- @param s number The number of seconds to represent.
    --- @return span A duration object representing the given seconds.
    seconds: (s: number) -> span,
    --- Creates a `span` representing the specified number of nanoseconds.
    --- @param ns number The number of nanoseconds to represent.
    --- @return span A duration object representing the given nanoseconds.
    nanoseconds: (ns: number) -> span,
    --- Creates a `span` representing the specified number of milliseconds.
    --- @param ms number The number of milliseconds to represent.
    --- @return span A duration object representing the given milliseconds.
    milliseconds: (ms: number) -> span,
    --- Creates a `span` representing the specified number of microseconds.
    --- @param us number The number of microseconds to represent.
    --- @return span A duration object representing the given microseconds.
    microseconds: (us: number) -> span,
    --- Creates a `span` representing the specified number of minutes.
    --- @param min number The number of minutes to represent.
    --- @return span A duration object representing the given minutes.
    minutes: (min: number) -> span,
    --- Creates a `span` representing the specified number of days.
    --- @param d number The number of days to represent.
    --- @return span A duration object representing the given days.
    days: (d: number) -> span,
    --- Creates a `span` representing the specified number of months.
    --- Note: The exact interpretation of a "month" may depend on your implementation.
    --- @param d number The number of months to represent.
    --- @return span A duration object representing the given months.
    months: (d: number) -> span,
    --- Creates a `span` representing the specified number of hours.
    --- @param h number The number of hours to represent.
    --- @return span A duration object representing the given hours.
    hours: (h: number) -> span,
}
return dlimport.require("time") :: time 
