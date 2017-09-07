//
// ulib - a collection of useful classes
// Copyright (C) 2017 Michael Fink
//
/// \file TestFileFinder.cpp tests FileFinder class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/FileFinder.hpp>
#include <ulib/Path.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests FileFinder class
   TEST_CLASS(TestFileFinder)
   {
   public:
      /// tests static method FindAllInPath()
      TEST_METHOD(FindAllInPath)
      {
         std::vector<CString> filenamesList =
            FileFinder::FindAllInPath(Path::SpecialFolder(CSIDL_COMMON_STARTMENU), _T("*.*"), false, true);

         Assert::IsFalse(filenamesList.empty(), _T("list must contain file names"));
      }
   };

} // namespace UnitTest
