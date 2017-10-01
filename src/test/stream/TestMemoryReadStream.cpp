//
// ulib - a collection of useful classes
// Copyright (C) 2007,2017 Michael Fink
//
/// \file TestMemoryReadStream.cpp tests for memory read-only stream
//

#include "stdafx.h"
#include <ulib/stream/MemoryReadStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests MemoryReadStream class
   TEST_CLASS(TestMemoryReadStream)
   {
      /// tests basic functionality
      TEST_METHOD(TestBasic)
      {
         BYTE abData[] = { 42, 128, 64 };

         Stream::MemoryReadStream ms(abData, sizeof(abData));

         Assert::IsTrue(true == ms.CanRead());
         Assert::IsTrue(false == ms.CanWrite());
         Assert::IsTrue(true == ms.CanSeek());

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

         Stream::MemoryReadStream ms(abData, sizeof(abData));

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

      /// tests seek functionality
      TEST_METHOD(TestSeek)
      {
         BYTE abData[] = { 42, 128, 64 };

         Stream::MemoryReadStream ms(abData, sizeof(abData));

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
