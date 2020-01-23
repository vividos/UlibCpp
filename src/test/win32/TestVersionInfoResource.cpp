//
// ulib - a collection of useful classes
// Copyright (C) 2018 Michael Fink
//
/// \file TestCommandLineParser.cpp tests for CommandLineParser class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/CommandLineParser.hpp>
#include <ulib/win32/VersionInfoResource.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests for VersionInfoResource class
   TEST_CLASS(TestVersionInfoResource)
   {
      /// tests loading version info resource
      TEST_METHOD(TestLoadVersionInfo)
      {
         // get module filename
         CString filename;
         GetModuleFileName(NULL, filename.GetBuffer(MAX_PATH), MAX_PATH);
         filename.ReleaseBuffer();

         // version info resource for current file
         Win32::VersionInfoResource vir(filename);
         Assert::IsTrue(vir.IsAvail());

         {
            Win32::FixedFileInfo* pFixedFileInfo = vir.GetFixedFileInfo();
            Assert::IsNotNull(pFixedFileInfo);
            Assert::IsTrue(pFixedFileInfo->dwSignature == 0xFEEF04BD);

            Win32::FixedFileInfo& fixedFileInfo = *pFixedFileInfo;

            // print out all fixed file infos
            CString text;
            text.Format(
               _T("fileversion=%u.%u.%u.%u productversion=%u.%u.%u.%u structver=%u.%u\n"),
               HIWORD(fixedFileInfo.dwFileVersionMS), LOWORD(fixedFileInfo.dwFileVersionMS),
               HIWORD(fixedFileInfo.dwFileVersionLS), LOWORD(fixedFileInfo.dwFileVersionLS),
               HIWORD(fixedFileInfo.dwProductVersionMS), LOWORD(fixedFileInfo.dwProductVersionMS),
               HIWORD(fixedFileInfo.dwProductVersionLS), LOWORD(fixedFileInfo.dwProductVersionLS),
               HIWORD(fixedFileInfo.dwStrucVersion), LOWORD(fixedFileInfo.dwStrucVersion));

            text.AppendFormat(
               _T("fileflags=%08x mask=%08x os=%08x type=%08x subtype=%08x date=0x%08x%08x\n"),
               fixedFileInfo.dwFileFlags,          // e.g. VFF_DEBUG | VFF_PRERELEASE
               fixedFileInfo.dwFileFlagsMask,      // = 0x3F for version "0.42"
               fixedFileInfo.dwFileOS,             // e.g. VOS_DOS_WINDOWS16
               fixedFileInfo.dwFileType,           // e.g. VFT_DRIVER
               fixedFileInfo.dwFileSubtype,        // e.g. VFT2_DRV_KEYBOARD
               fixedFileInfo.dwFileDateMS,         // e.g. 0
               fixedFileInfo.dwFileDateLS);        // e.g. 0

            text.AppendFormat(
               _T("os=%s type=%s\n"),
               fixedFileInfo.GetFileOS().GetString(),
               fixedFileInfo.GetFileType().GetString());

            text.AppendFormat(
               _T("fileversion=%s productversion=%s"),
               fixedFileInfo.GetFileVersion().GetString(),
               fixedFileInfo.GetProductVersion().GetString());

            ATLTRACE(text);
         }

         // show languages and codepages
         {
            std::vector<Win32::LANGANDCODEPAGE> langAndCodepagesList;
            vir.GetLangAndCodepages(langAndCodepagesList);

            Assert::IsTrue(langAndCodepagesList.size() > 0);

            // print out all languages and codepages
            for (size_t langIndex = 0; langIndex < langAndCodepagesList.size(); langIndex++)
            {
               CString text;
               text.Format(_T("language %04x [%d] codepage %04x [%d]"),
                  langAndCodepagesList[langIndex].wLanguage,
                  langAndCodepagesList[langIndex].wLanguage,
                  langAndCodepagesList[langIndex].wCodePage,
                  langAndCodepagesList[langIndex].wCodePage);

               ATLTRACE(text);

               // show all language-dependent strings
               LPCTSTR allNames[] =
               {
                  _T("Comments"),
                  _T("CompanyName"),
                  _T("FileDescription"),
                  _T("FileVersion"),
                  _T("InternalName"),
                  _T("LegalCopyright"),
                  _T("LegalTrademarks"),
                  _T("OriginalFilename"),
                  _T("ProductName"),
                  _T("ProductVersion"),
                  _T("PrivateBuild"),
                  _T("SpecialBuild")
               };

               for (unsigned int nameIndex = 0; nameIndex < sizeof_array(allNames); nameIndex++)
               {
                  CString text2;
                  text2.Format(_T("  name \"%-17s: [%s]"),
                     (CString(allNames[nameIndex]) + _T("\"")).GetString(),
                     vir.GetStringValue(langAndCodepagesList[langIndex], allNames[nameIndex]).GetString());

                  ATLTRACE(text2);
               }
            }
         }
      }
   };

} // namespace UnitTest
