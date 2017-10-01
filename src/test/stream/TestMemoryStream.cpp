//
// ulib - a collection of useful classes
// Copyright (C) 2007,2017 Michael Fink
//
/// \file TestMemoryStream.cpp tests for memory read-write stream
//

#include "stdafx.h"
#include <ulib/stream/MemoryStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests MemoryStream class
   TEST_CLASS(TestMemoryStream)
   {
      /// tests basic functionality
      TEST_METHOD(TestBasic)
      {
         // default ctor
         {
            Stream::MemoryStream ms;
            Assert::IsTrue(0 == ms.GetData().size());
         }

         BYTE abData[] = { 42, 128, 64 };

         // misc. functions
         Stream::MemoryStream ms(abData, sizeof(abData));

         Assert::IsTrue(true == ms.CanRead());
         Assert::IsTrue(true == ms.CanWrite());
         Assert::IsTrue(true == ms.CanSeek());

         Assert::IsTrue(3 == ms.GetData().size());
         Assert::IsTrue(abData[0] == ms.GetData()[0]);
         Assert::IsTrue(abData[1] == ms.GetData()[1]);
         Assert::IsTrue(abData[2] == ms.GetData()[2]);

         Assert::IsTrue(false == ms.AtEndOfStream());

         Assert::IsTrue(0LL == ms.Position());
         Assert::IsTrue(3LL == ms.Length());

         ms.Flush();
         ms.Close();
      }

      /// tests read functionality
      TEST_METHOD(TestRead)
      {
         BYTE abData[] = { 42, 128, 64 };

         Stream::MemoryStream ms(abData, sizeof(abData));

         Assert::IsTrue(true == ms.CanRead());

         BYTE bFill = 42;
         BYTE abBuffer[4];
         memset(abBuffer, bFill, sizeof(abBuffer));

         // read in 1 byte
         {
            DWORD dwRead = (DWORD)-1;
            Assert::IsTrue(true == ms.Read(abBuffer, 1, dwRead));
            Assert::IsTrue(1 == dwRead);

            Assert::IsTrue(abBuffer[0] == abData[0]);
            Assert::IsTrue(abBuffer[1] == bFill);
         }

         Assert::IsTrue(false == ms.AtEndOfStream());

         memset(abBuffer, bFill, sizeof(abBuffer));

         // read in 3 byte, with only 2 left
         {
            DWORD dwRead = (DWORD)-1;
            Assert::IsTrue(true == ms.Read(abBuffer, 3, dwRead));
            Assert::IsTrue(2 == dwRead);

            Assert::IsTrue(abBuffer[0] == abData[1]);
            Assert::IsTrue(abBuffer[1] == abData[2]);
            Assert::IsTrue(abBuffer[2] == bFill);
         }

         Assert::IsTrue(true == ms.AtEndOfStream());

         memset(abBuffer, bFill, sizeof(abBuffer));

         // next read should return false, since there are no more bytes
         {
            DWORD dwRead = (DWORD)-1;
            Assert::IsTrue(false == ms.Read(abBuffer, 1, dwRead));
            Assert::IsTrue(0 == dwRead);

            Assert::IsTrue(abBuffer[0] == bFill);
         }

         Assert::IsTrue(true == ms.AtEndOfStream());
      }

      /// tests write functionality
      TEST_METHOD(Write)
      {
         BYTE abData[] = { 42, 128, 64 };

         Stream::MemoryStream ms(abData, sizeof(abData));

         Assert::IsTrue(true == ms.CanWrite());

         // overwrite byte at pos 0
         {
            ms.WriteByte(abData[0] + 1);
            Assert::IsTrue(abData[0] + 1 == ms.GetData()[0]);
         }

         // overwrite with extending memory stream
         {
            Assert::IsTrue(2ULL == ms.Seek(2LL, Stream::IStream::seekBegin));

            DWORD dwWritten = (DWORD)-1;
            ms.Write(abData, 2, dwWritten);
            Assert::IsTrue(2 == dwWritten);

            Assert::IsTrue(4ULL == ms.Length());
            Assert::IsTrue(4ULL == ms.Position());
            Assert::IsTrue(true == ms.AtEndOfStream());

            Assert::IsTrue(abData[0] == ms.GetData()[2]);
            Assert::IsTrue(abData[1] == ms.GetData()[3]);
         }
      }

      /// tests seek functionality
      TEST_METHOD(TestSeek)
      {
         BYTE abData[] = { 42, 128, 64 };

         Stream::MemoryStream ms(abData, sizeof(abData));

         Assert::IsTrue(true == ms.CanSeek());

         // test seek from begin
         {
            Assert::IsTrue(1ULL == ms.Seek(1LL, Stream::IStream::seekBegin));
            Assert::IsTrue(1ULL == ms.Position());

            Assert::IsTrue(abData[1] == ms.ReadByte());

            Assert::IsTrue(2ULL == ms.Position());

            Assert::IsTrue(3ULL == ms.Seek(4LL, Stream::IStream::seekBegin));
         }

         // test seek from end
         {
            Assert::IsTrue(2ULL == ms.Seek(1LL, Stream::IStream::seekEnd));
            Assert::IsTrue(2ULL == ms.Position());

            Assert::IsTrue(abData[2] == ms.ReadByte());

            Assert::IsTrue(3LL == ms.Position());

            Assert::IsTrue(true == ms.AtEndOfStream());

            Assert::IsTrue(1ULL == ms.Seek(2LL, Stream::IStream::seekEnd));
            Assert::IsTrue(0ULL == ms.Seek(4LL, Stream::IStream::seekEnd));
         }

         // test seek from current position
         {
            Assert::IsTrue(1ULL == ms.Seek(1LL, Stream::IStream::seekBegin));

            Assert::IsTrue(2ULL == ms.Seek(1LL, Stream::IStream::seekCurrent));
            Assert::IsTrue(1ULL == ms.Seek(-1LL, Stream::IStream::seekCurrent));

            Assert::IsTrue(0ULL == ms.Seek(-2LL, Stream::IStream::seekCurrent));
            Assert::IsTrue(3ULL == ms.Seek(4LL, Stream::IStream::seekCurrent));
         }
      }
   };

} // namespace UnitTest
