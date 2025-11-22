# UlibCpp Version Migration Guide

## Version 4.x -> 5.x

There are some breaking changes between 4.x and 5.x.

### C++20

It is required to compile your project with C++20 in order to use the library.
If you have an ATL or WTL project, be sure to also set "Conformance Mode" to
"No (/permissive) since ATL is not prepared for C++20.

### CommandLine and ProgramOptions

The `CommandLine` and `ProgramOptions` classes now take an `LPCTSTR argv[]`
parameter instead of `_TCHAR* argv[]` to be more const aware. No changes are
necessary when using these classes.

### DateTime and TimeSpan

The `DateTime` and `TimeSpan` classes now are using `std::chrono`, and because
of that, some breaking changes were made. The two classes are (mostly) defined
in their headers and don't use a `shared_ptr` to hold the implementation now.

### `DateTime` changes

- The `DateTime` implementation is now mostly header-only for easier inlining
  and has a size of 16 bytes (depending on alignment).
- The `DateTime` ctor now optionally takes a `std::chrono::time_point` time
  point object.

### `TimeSpan` changes

- The `TimeSpan` implementation is now header-only and has a size of 8 bytes.
- The default constructed `TimeSpan` object has now zero duration, not an
  invalid status anymore.
- As a time span now always is valid, the `TimeSpan::Status()` method and
  the ctor with `T_enStatus` parameter were removed as well. If a "non-valid"
  time span is required, use `std::optional<TimeSpan>` instead.
- The `TimeSpan` ctor now optionally takes a `std::chrono::milliseconds`
  duration object.
- The `Total*()` methods like `TotalHours()` returned `double` values but
  always truncated it to integer values; this is fixed.
- The already deprecated `FormatISO8601()` method was removed. Use
 `Format(T_enTimeSpanFormat::formatISO8601)` instead.
- The method `Format(LPCTSTR)` was removed as well. Use the `TimeSpan`
 components (like `Hours()`, etc.) directly to use a custom format.
