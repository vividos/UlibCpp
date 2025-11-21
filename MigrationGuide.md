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
