# UlibCpp

## Description

UlibCpp is a collection of useful C++ classes that help writing Win32 console, ATL and WTL applications faster.

Documentation for UlibCpp can be found here:
[Documentation.md](Documentation.md "UlibCpp Documentation").

The changelog for UlibCpp can be found here:
[Changelog.md](Changelog.md "UlibCpp Changelog").

Install the NuGet package to use UlibCpp right away: Vividos.UlibCpp.Static

## Build status

NuGet: [![NuGet package](https://img.shields.io/nuget/v/Vividos.UlibCpp.Static.svg)](https://www.nuget.org/packages/Vividos.UlibCpp.Static/)

AppVeyor: [![Build status](https://ci.appveyor.com/api/projects/status/barfr2cil647t8du?svg=true)](https://ci.appveyor.com/project/vividos/ulibcpp)

SonarCloud: [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=UlibCpp&metric=alert_status)](https://sonarcloud.io/dashboard?id=UlibCpp)

## License

Licensed under the [MIT license](LICENSE.md).

## History

UlibCpp (or then called "ulib") started as a folder with include files that I
often use in projects. At some time I decided to create a static library as
well as a DLL that contains the classes that were not header-only. Versions 1
and 2 were compiled under Win32 and also for the Windows Mobile platform, as I
wrote command line and WTL apps for those platforms.

Versions 1 and 2 had many more classes that were only used in one project. In
version 3 I started a new solution and project and tried to keep the classes
at the lowest common denominator. Also the Windows Mobile platform wasn't as
relevant now. I kept (and copied) the ulib files in several repositories where
they tended to drift apert.

When discovering that NuGet also works for C++ libraries, I decided to do a
new version of ulib, this time named UlibCpp, to differentiate from all the
C# NuGet packages. Over time I added some more classes and methods from the
various projects I'm publicly developing on GitHub.

Currently the following projects of mine use UlibCpp:
- [winLAME](https://github.com/vividos/winLAME)
- [RemotePhotoTool](https://github.com/vividos/RemotePhotoTool)
- Some projects in the [OldStuff](https://github.com/vividos/OldStuff) repository
