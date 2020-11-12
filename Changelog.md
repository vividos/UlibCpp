# UlibCpp Changelog

# Version 4.2.2

Compiled with Visual Studio 2019 Version 16.7.7 and Boost 1.72.

This version suppresses more warnings when including Boost and WTL headers.

New classes: SystemImageList

# Version 4.2.1

Compiled with Visual Studio 2019 Version 16.5.0 and Boost 1.71.

- Fixed crash in Win32::Taskbar class when internal ITaskbarList3 COM object
 couldn't be accessed

# Version 4.2.0

Compiled with Visual Studio 2019 Version 16.4.3 and Boost 1.71.

No new classes. Some classes were changed to include move ctors and move
assignment operators.

- Path contains a mixture of object methods and static methods; the object
  methods are now marked deprecated, to be removed in version 4.3.0.
  Warning C4996 will show what calls have to be adjusted to the equivalent
  static methods.
- Method Clipboard::SetHtml() was deprecated, as it was never implemented
- Fixed some edge cases when parsing program options in ProgramOptions
- Stream::FileStream now caches the file length when calling Length()
- Added TimeSpan::Format() and marked FormatISO8601() obsolete

# Version 4.1.7

Compiled with Visual Studio 2019 Version 16.4.1 and Boost 1.70.

No new classes or functions.

# Version 4.1.6

Compiled with Visual Studio 2019 Version 16.1.4 and Boost 1.70.

No new classes or functions.

# Version 4.1.5

Compiled with Visual Studio 2017 Version 15.9.5 and Boost 1.68.

This version suppresses more warnings when including Boost and WTL headers. Also:

- made MutexLock, MutexTryLock and MutexUnLocker movable
- fixed including Singleton.hpp when compiling for C++17

# Version 4.1.4

Compiled with Visual Studio 2017 Version 15.8.9 and Boost 1.68.

This version suppresses warning 4996 about deprecated features when compiling
with C++17 and including `ulib\config\BoostAsio.hpp`.

# Version 4.1.3

Compiled with Visual Studio 2017 Version 15.8.7 and Boost 1.68. Use this
package together with the WTL 10.0.8280 or later package.

# Version 4.1.2

Compiled with Visual Studio 2017 Version 15.8.3 and Boost 1.68.

New classes: VersionInfoResource

New methods: sizeof_array, Win32::ErrorMessage::LastError()

# Version 4.1.1

Compiled with Visual Studio 2017 Version 15.5.2 and Boost 1.66.

New methods: Path::ModuleFilename()

Updated atlribbon.h for VS 2017 that is automatically included in config/Atl.hpp

# Version 4.1.0

Compiled with Visual Studio 2017 Update 3 and Boost 1.65.1.

New classes: Clipboard, MemoryReadStream, NullStream, HighResolutionTimer, Win7Taskbar, TraceOutputStopwatch, Singleton

New methods in Path, FileFinder and ProgramOptions

Bug fixes and more unit tests for existing classes.

# Version 4.0.1

Compatibility changes, bug fixes and changes to remove SonarQube and Coverity Scan warnings.

# Version 4.0.0

Compiled with Visual Studio 2017 Update 3 and Boost 1.65.0.

This is the initial release of the library.
