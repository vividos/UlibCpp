//
// ulib - a collection of useful classes
// Copyright (C) 2017,2020 Michael Fink
//
/// \file TestPath.cpp Unit tests for Path class
//

#include "stdafx.h"
#include "CppUnitTest.h"
#include <ulib/Path.hpp>
#include <ulib/unittest/AutoCleanupFolder.hpp>
#include <ulib/stream/FileStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// Tests Path class
   TEST_CLASS(TestPath)
   {
   public:
      /// Tests FilenameAndExt() method
      TEST_METHOD(TestFilenameAndExt)
      {
         Assert::AreEqual(_T("temp.txt"), Path::FilenameAndExt(_T("C:\\win\\desktop\\temp.txt")), "filename must be temp.txt");
         Assert::AreEqual(_T("temp."), Path::FilenameAndExt(_T("C:\\win\\desktop\\temp.")), "filename must be temp.");
         Assert::AreEqual(_T("three"), Path::FilenameAndExt(_T("E:\\acme\\three")), "filename must be three");
         Assert::AreEqual(_T(""), Path::FilenameAndExt(_T("E:\\acme\\three\\")), "filename must be empty");
      }

      /// Tests FilenameOnly() method
      TEST_METHOD(TestFilenameOnly)
      {
         Assert::AreEqual(_T("temp"), Path::FilenameOnly(_T("C:\\win\\desktop\\temp.txt")), "filename must be temp");
         Assert::AreEqual(_T("temp.temp"), Path::FilenameOnly(_T("C:\\win\\desktop\\temp.temp.txt")), "filename must be temp.temp");
         Assert::AreEqual(_T("temp"), Path::FilenameOnly(_T("C:\\win\\desktop\\temp.")), "filename must be temp");
         Assert::AreEqual(_T("three"), Path::FilenameOnly(_T("E:\\acme\\three")), "filename must be three");
         Assert::AreEqual(_T(""), Path::FilenameOnly(_T("E:\\acme\\three\\")), "filename must be empty");
         Assert::AreEqual(_T(""), Path::FilenameOnly(_T("E:\\acme\\.gitignore")), "filename must be empty");
      }

      /// Tests ExtensionOnly() method
      TEST_METHOD(TestExtensionOnly)
      {
         Assert::AreEqual(_T(".ext"), Path::ExtensionOnly(_T("filename.ext")), "extension must be .ext");
         Assert::AreEqual(_T(""), Path::ExtensionOnly(_T("filename")), "extension must be empty");
         Assert::AreEqual(_T("."), Path::ExtensionOnly(_T("filename.")), "extension must be .");
         Assert::AreEqual(_T(""), Path::ExtensionOnly(_T("c:\\my.test\\filename")), "extension must be empty");
         Assert::AreEqual(_T(".gitignore"), Path::ExtensionOnly(_T("E:\\acme\\.gitignore")), "extension must be .gitignore");
      }

      /// Tests FolderName() method
      TEST_METHOD(TestFolderName)
      {
         Assert::AreEqual(_T("C:\\win\\desktop\\"), Path::FolderName(_T("C:\\win\\desktop\\temp.txt")), "folder name must be correct");
         Assert::AreEqual(_T("C:\\win\\desktop\\"), Path::FolderName(_T("C:\\win\\desktop\\temp.")), "folder name must be correct");
         Assert::AreEqual(_T("E:\\acme\\"), Path::FolderName(_T("E:\\acme\\three")), "folder name must be correct");
      }

      /// Tests ShortPathName() method
      TEST_METHOD(TestShortPathName)
      {
         // set up
         AutoCleanupFolder folder;
         CString filename = Path::Combine(folder.FolderName(), "longfilename.txt");

         // ShortPathName() only works when the file actually exists
         Stream::FileStream fs{
            filename,
            Stream::FileStream::modeCreateNew,
            Stream::FileStream::accessWrite,
            Stream::FileStream::shareRead };
         fs.Close();

         // run
         CString shortPath = Path::ShortPathName(filename);
         shortPath.MakeLower();

         // check
         Assert::AreEqual(_T("longfi~1.txt"), Path::FilenameAndExt(shortPath), "short filename must be correct");
      }

      /// Tests MakeRelativeTo() method
      TEST_METHOD(TestMakeRelativeTo)
      {
         // set up
         CString rootPath = _T("C:\\win\\desktop");

         // run
         CString relativePath1 = Path::MakeRelativeTo(_T("C:\\win\\desktop\\temp.txt"), rootPath);
         CString relativePath2 = Path::MakeRelativeTo(_T("C:\\win\\desktop\\subfolder\\temp.txt"), rootPath);
         CString relativePath3 = Path::MakeRelativeTo(_T("C:\\win\\temp.txt"), rootPath);
         CString relativePath4 = Path::MakeRelativeTo(_T("d:\\otherfolder\\temp.txt"), rootPath);

         // check
         Assert::AreEqual(_T(".\\temp.txt"), relativePath1, L"relative path must be correct");
         Assert::AreEqual(_T(".\\subfolder\\temp.txt"), relativePath2, L"relative path must be correct");
         Assert::AreEqual(_T("..\\temp.txt"), relativePath3, L"relative path must be correct");
         Assert::IsTrue(relativePath4.IsEmpty(), L"relative path must be empty");
      }

      /// Tests for IsRelative() method
      TEST_METHOD(TestIsRelative)
      {
         // check
         Assert::IsTrue(Path::IsRelative(_T(".\\temp.txt")), L"path must be relative");
         Assert::IsFalse(Path::IsRelative(_T("c:\\win\\desktop\\temp.txt")), L"path must not be relative");
         Assert::IsFalse(Path::IsRelative(_T("c:\\win\\desktop\\..\\temp.txt")), L"path must not be relative");
      }

      /// Tests for FileExists() method
      TEST_METHOD(TestFileExists)
      {
         // set up
         AutoCleanupFolder folder;
         CString filename = Path::Combine(folder.FolderName(), "testfilename.txt");

         // check 1
         Assert::IsFalse(Path::FileExists(folder.FolderName()), L"file must not exist, since it's a folder");

         Assert::IsFalse(Path::FileExists(filename), L"file must not exist yet");

         // run
         Stream::FileStream fs{
            filename,
            Stream::FileStream::modeCreateNew,
            Stream::FileStream::accessWrite,
            Stream::FileStream::shareRead };
         fs.Close();

         // check 2
         Assert::IsTrue(Path::FileExists(filename), L"file must exist now");
         Assert::IsFalse(Path::FileExists(folder.FolderName()), L"file must not exist, since it's still a folder");
      }

      /// Tests for FolderExists() method
      TEST_METHOD(TestFolderExists)
      {
         // set up
         AutoCleanupFolder folder;
         CString subfolderName = Path::Combine(folder.FolderName(), "testfolder");

         // check 1
         Assert::IsFalse(Path::FileExists(folder.FolderName()), L"file must not exist, since it's a folder");
         Assert::IsTrue(Path::FolderExists(folder.FolderName()), L"folder must exist");
         Assert::IsFalse(Path::FolderExists(subfolderName), L"subfolder must not exist yet");

         // run
         Assert::IsTrue(Path::CreateDirectoryRecursive(subfolderName), L"creating subfolder must succeed");

         // check 2
         Assert::IsTrue(Path::FolderExists(subfolderName), L"subfolder must exist now");
      }

      /// Tests for GetCommonRootPath() method
      TEST_METHOD(TestGetCommonRootPath)
      {
         CString path1 = _T("C:\\win\\desktop\\temp.txt");
         CString path2 = _T("c:\\win\\tray\\sample.txt");
         CString path3 = _T("E:\\acme\\three");

         Assert::AreEqual(_T("C:\\win\\"), Path::GetCommonRootPath(path1, path2));
         Assert::AreEqual(_T(""), Path::GetCommonRootPath(path1, path3));
         Assert::AreEqual(_T(""), Path::GetCommonRootPath(path2, path3));

         Assert::AreEqual(_T("C:\\win\\desktop\\"), Path::GetCommonRootPath(path1, _T("C:\\win\\desktop\\")));
         Assert::AreEqual(_T("C:\\win\\desktop\\"), Path::GetCommonRootPath(path1, _T("C:\\win\\desktop")));

         Assert::AreEqual(_T("E:\\acme\\three\\"), Path::GetCommonRootPath(path3, path3));
      }

      /// Tests SpecialFolder() method
      TEST_METHOD(TestSpecialFolder)
      {
         CString folder = Path::SpecialFolder(CSIDL_COMMON_APPDATA);

         Assert::IsFalse(folder.IsEmpty(), L"path must not be empty");
         Assert::IsTrue(Path::FolderExists(folder), L"folder must exist");
      }

      /// Tests WindowsFolder() method
      TEST_METHOD(TestWindowsFolder)
      {
         CString folder = Path::WindowsFolder();

         Assert::IsFalse(folder.IsEmpty(), L"path must not be empty");
         Assert::IsTrue(Path::FolderExists(folder), L"folder must exist");
      }

      /// Tests TempFolder() method
      TEST_METHOD(TestTempFolder)
      {
         CString folder = Path::TempFolder();

         Assert::IsFalse(folder.IsEmpty(), L"path must not be empty");
         Assert::IsTrue(Path::FolderExists(folder), L"folder must exist");
      }

      /// Tests ModuleFilename() method
      TEST_METHOD(TestModuleFilename)
      {
         CString filenameCurrentModule = Path::ModuleFilename();
         CString filenameKernel32Dll = Path::ModuleFilename(GetModuleHandle(_T("kernel32.dll")));

         Assert::IsFalse(filenameCurrentModule.IsEmpty());
         Assert::IsFalse(filenameKernel32Dll.IsEmpty());
      }

      /// Tests for CreateDirectoryRecursive() method
      TEST_METHOD(TestCreateDirectoryRecursive)
      {
         // set up
         AutoCleanupFolder folder;
         CString subfolderName = Path::Combine(folder.FolderName(), "test");
         CString subSubFolderName = Path::Combine(subfolderName, "folder");

         // check 1
         Assert::IsTrue(Path::FolderExists(folder.FolderName()), L"folder must exist");
         Assert::IsFalse(Path::FolderExists(subfolderName), L"subfolder must not exist yet");
         Assert::IsFalse(Path::FolderExists(subSubFolderName), L"sub-subfolder must not exist yet");

         // run
         Assert::IsTrue(Path::CreateDirectoryRecursive(subSubFolderName), L"creating subfolder must succeed");

         // check 2
         Assert::IsTrue(Path::FolderExists(folder.FolderName()), L"folder must exist");
         Assert::IsTrue(Path::FolderExists(subfolderName), L"subfolder must exist now");
         Assert::IsTrue(Path::FolderExists(subSubFolderName), L"sub-subfolder must exist now");
      }
   };
}
