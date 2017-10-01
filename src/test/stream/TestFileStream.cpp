//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2017 Michael Fink
//
/// \file TestFileStream.cpp unit tests for file streams
//

#include "stdafx.h"
#include <ulib/stream/StreamException.hpp>
#include <ulib/unittest/AutoCleanupFolder.hpp>
#include <ulib/stream/FileStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using Stream::FileStream;

namespace UnitTest
{
   /// tests FileStream class
   TEST_CLASS(TestFileStream)
   {
      /// creates testfile for tests
      bool CreateTestFile(const CString& cszFilename);

      /// tests FileStream class, read functionality
      TEST_METHOD(TestRead1);

      /// tests creating copies of FileStream objects
      TEST_METHOD(TestCopyCtor);

      // tests FileStream class, open functionality
      TEST_METHOD(TestOpen1);
      TEST_METHOD(TestOpen2);
      TEST_METHOD(TestOpen3);
      TEST_METHOD(TestOpen4);
      TEST_METHOD(TestOpen5);
      TEST_METHOD(TestOpen6);

      // tests FileStream class, seek functionality
      TEST_METHOD(TestSeek1);
      TEST_METHOD(TestSeek2);
      TEST_METHOD(TestSeek3);

      // tests FileStream class, AtEndOfStream function
      TEST_METHOD(TestAtEndOfStream);
   };

} // namespace UnitTest

using UnitTest::TestFileStream;

/// creates test file
bool TestFileStream::CreateTestFile(const CString& cszFilename)
{
   FileStream fs(cszFilename, FileStream::modeCreateNew, FileStream::accessWrite, FileStream::shareNone);

   if (!fs.IsOpen())
      return false;

   BYTE abData[] =
   {
      0x0c, 0x64, 0x15, 0x41, 0x42, 0xff
   };

   DWORD dwWritten = 0;
   fs.Write(abData, sizeof(abData), dwWritten);
   if (dwWritten != sizeof(abData))
      return false;

   return true;
}

/// tests reading from file
void TestFileStream::TestRead1()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   Assert::IsTrue(CreateTestFile(cszFilename));

   FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

   Assert::IsTrue(fs.IsOpen());
   Assert::IsFalse(fs.AtEndOfStream());
   Assert::IsTrue(fs.CanRead());

   Assert::IsTrue(0 != fs.ReadByte());
   Assert::IsFalse(fs.AtEndOfStream());

   BYTE abBuffer[8] = { 0 };
   DWORD dwReadBytes = 0;
   Assert::IsTrue(fs.Read(abBuffer, 4, dwReadBytes));
   Assert::IsTrue(4 == dwReadBytes);

   Assert::IsFalse(fs.AtEndOfStream());

   dwReadBytes = 0;
   Assert::IsTrue(fs.Read(abBuffer, 2, dwReadBytes));
   Assert::IsTrue(1 == dwReadBytes);

   Assert::IsTrue(fs.AtEndOfStream());
}

/// tests copy constructor and IsOpen()
void TestFileStream::TestCopyCtor()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   Assert::IsTrue(CreateTestFile(cszFilename));

   FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

   Assert::IsTrue(fs.IsOpen());

   FileStream fs2 = fs;

   Assert::IsTrue(fs.IsOpen());
   Assert::IsTrue(fs2.IsOpen());

   fs2.Close();

   Assert::IsTrue(fs.IsOpen());
   Assert::IsFalse(fs2.IsOpen());

   fs.Close();

   Assert::IsFalse(fs.IsOpen());
   Assert::IsFalse(fs2.IsOpen());
}

/// tests modeCreateNew; fails when file already exists
void TestFileStream::TestOpen1()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // create file; shouldn't exist yet
   {
      FileStream fs(cszFilename, FileStream::modeCreateNew, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(42);
   }

   // create file; should fail, since file exists now
   try
   {
      FileStream fs(cszFilename, FileStream::modeCreateNew, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(false);
   }
   catch (Stream::StreamException& e)
   {
      (e);
   }
}

/// tests modeCreate; if it already exists, it is overwritten
void TestFileStream::TestOpen2()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // create file; shouldn't exist yet
   {
      FileStream fs(cszFilename, FileStream::modeCreate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(42);
   }

   // create file; should overwrite file
   {
      FileStream fs(cszFilename, FileStream::modeCreate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(53);
   }

   // read test file
   {
      FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
      Assert::IsTrue(fs.IsOpen());

      Assert::IsTrue(53 == fs.ReadByte());
   }
}

/// tests modeOpen; if it doesn't exist yet, open fails
void TestFileStream::TestOpen3()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // open file; shouldn't exist yet, so should fail
   try
   {
      FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
      Assert::IsTrue(false);
   }
   catch (Stream::StreamException& e)
   {
      (e);
   }

   // create file
   {
      FileStream fs(cszFilename, FileStream::modeCreate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(53);
   }

   // open file; file exists now
   {
      FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
      Assert::IsTrue(fs.IsOpen());

      Assert::IsTrue(53 == fs.ReadByte());
   }
}

/// tests modeOpenOrCreate; if it doesn't exist yet, open creates a new file
void TestFileStream::TestOpen4()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // open file; shouldn't exist yet, so should create a new file
   {
      FileStream fs(cszFilename, FileStream::modeOpenOrCreate, FileStream::accessReadWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(53);
   }

   // open file; file exists now
   {
      FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
      Assert::IsTrue(fs.IsOpen());

      Assert::IsTrue(53 == fs.ReadByte());
   }
}

/// tests modeTruncate; should fail when no file to open is
void TestFileStream::TestOpen5()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // create file; shouldn't exist yet, so should fail
   try
   {
      FileStream fs(cszFilename, FileStream::modeTruncate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(false);
   }
   catch (Stream::StreamException& e)
   {
      (e);
   }

   // create file
   {
      FileStream fs(cszFilename, FileStream::modeCreate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(53);
      Assert::IsTrue(1 == fs.Length());
   }

   // create file; should exist now
   {
      FileStream fs(cszFilename, FileStream::modeTruncate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      Assert::IsTrue(0 == fs.Position());
      Assert::IsTrue(0 == fs.Length());

      fs.WriteByte(42);
      Assert::IsTrue(1 == fs.Length());
   }

   // open file; check file contents
   {
      FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
      Assert::IsTrue(fs.IsOpen());
      Assert::IsTrue(1 == fs.Length());

      Assert::IsTrue(42 == fs.ReadByte());
   }
}

/// tests modeAppend; seeks to the end of the file
void TestFileStream::TestOpen6()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // create file; shouldn't exist yet
   {
      FileStream fs(cszFilename, FileStream::modeCreate, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(42);
   }

   // append file
   {
      FileStream fs(cszFilename, FileStream::modeAppend, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      fs.WriteByte(53);
   }

   // append file again, trying to seek before position 2
   {
      FileStream fs(cszFilename, FileStream::modeAppend, FileStream::accessWrite, FileStream::shareNone);
      Assert::IsTrue(fs.IsOpen());

      // seek to pos 2
      Assert::IsTrue(2 == fs.Seek(2L, Stream::IStream::seekBegin));
      // seek to pos 1, should fail
      // note: doesn't fail, since seeking before previous file end works in windows
      Assert::IsTrue(1 == fs.Seek(1L, Stream::IStream::seekBegin));
   }

   // read test file
   {
      FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
      Assert::IsTrue(fs.IsOpen());

      Assert::IsTrue(42 == fs.ReadByte());
      Assert::IsTrue(53 == fs.ReadByte());
   }
}

/// tests seeking from beginning
void TestFileStream::TestSeek1()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   Assert::IsTrue(CreateTestFile(cszFilename));

   FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
   Assert::IsTrue(fs.IsOpen());

   Assert::IsTrue(6 == fs.Length());
   Assert::IsTrue(0 == fs.Position());

   // seeking to mid position
   Assert::IsTrue(3 == fs.Seek(3, Stream::IStream::seekBegin));
   Assert::IsTrue(3 == fs.Position());

   // seeking beyond end
   // note: seeking to position 8 sets the pointer to the actual position 8,
   // but the file is only increased when writing to the file; the bytes in
   // between would be uninitialized then
   Assert::IsTrue(8 == fs.Seek(8, Stream::IStream::seekBegin));
   Assert::IsTrue(8 == fs.Position());

   // seeking back
   Assert::IsTrue(2 == fs.Seek(2, Stream::IStream::seekBegin));
   Assert::IsTrue(2 == fs.Position());

   // seek to negative index
   // note: file pointer stays at its current location
   try
   {
      Assert::IsTrue(2 == fs.Seek(-1, Stream::IStream::seekBegin));
      Assert::IsTrue(false);
   }
   catch (Stream::StreamException& e)
   {
      (e);
   }

   Assert::IsTrue(2 == fs.Position());
}

/// tests seeking from current pos
void TestFileStream::TestSeek2()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   Assert::IsTrue(CreateTestFile(cszFilename));

   FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
   Assert::IsTrue(fs.IsOpen());

   // current pos: 0
   Assert::IsTrue(6 == fs.Length());
   Assert::IsTrue(0 == fs.Position());

   // seek to mid position
   Assert::IsTrue(3 == fs.Seek(3, Stream::IStream::seekCurrent));
   Assert::IsTrue(3 == fs.Position());

   // seek to next position
   Assert::IsTrue(4 == fs.Seek(1, Stream::IStream::seekCurrent));
   Assert::IsTrue(4 == fs.Position());

   // seek to previous position
   Assert::IsTrue(2 == fs.Seek(-2, Stream::IStream::seekCurrent));
   Assert::IsTrue(2 == fs.Position());

   // seek to negative index; should fail
   try
   {
      Assert::IsTrue(2 == fs.Seek(-3, Stream::IStream::seekCurrent));
      Assert::IsTrue(false);
   }
   catch (Stream::StreamException& e)
   {
      (e);
   }
   Assert::IsTrue(2 == fs.Position());

   // seek to after file end; should set pos to after file
   Assert::IsTrue(9 == fs.Seek(7, Stream::IStream::seekCurrent));
   Assert::IsTrue(9 == fs.Position());

   // seek with negative index
   Assert::IsTrue(5 == fs.Seek(-4, Stream::IStream::seekCurrent));
   Assert::IsTrue(5 == fs.Position());
}

/// tests seeking from end
void TestFileStream::TestSeek3()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   Assert::IsTrue(CreateTestFile(cszFilename));

   FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
   Assert::IsTrue(fs.IsOpen());

   Assert::IsTrue(6 == fs.Length());
   Assert::IsTrue(0 == fs.Position());

   // seek to mid position
   Assert::IsTrue(4 == fs.Seek(-2, Stream::IStream::seekEnd));
   Assert::IsTrue(4 == fs.Position());

   // seek to before start; should fail
   try
   {
      Assert::IsTrue(4 == fs.Seek(-8, Stream::IStream::seekEnd));
      Assert::IsTrue(false);
   }
   catch (Stream::StreamException& e)
   {
      (e);
   }
   Assert::IsTrue(4 == fs.Position());

   // seek to start
   Assert::IsTrue(0 == fs.Seek(-6, Stream::IStream::seekEnd));
   Assert::IsTrue(0 == fs.Position());

   // seek after file end
   Assert::IsTrue(8 == fs.Seek(2, Stream::IStream::seekEnd));
   Assert::IsTrue(8 == fs.Position());
}

/// tests AtEndOfStream function
void TestFileStream::TestAtEndOfStream()
{
   UnitTest::AutoCleanupFolder folder;
   CString cszFilename(folder.FolderName());
   cszFilename += _T("test.bin");

   // create a one-byte file
   {
      FileStream fs(cszFilename, FileStream::modeCreateNew, FileStream::accessWrite, FileStream::shareNone);
      fs.WriteByte(42);
   }

   FileStream fs(cszFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareWrite);
   Assert::IsTrue(fs.IsOpen());

   Assert::IsFalse(fs.AtEndOfStream());

   Assert::IsTrue(42 == fs.ReadByte());
   Assert::IsTrue(fs.AtEndOfStream());
}
