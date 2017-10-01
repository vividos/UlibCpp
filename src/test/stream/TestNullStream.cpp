//
// ulib - a collection of useful classes
// Copyright (C) 2007,2017 Michael Fink
//
/// \file TestMemoryStream.cpp tests for memory read-write stream
//

#include "stdafx.h"
#include <ulib/stream/NullStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests NullStream class
   TEST_CLASS(TestNullStream)
   {
      /// tests all NullStream functions
      TEST_METHOD(TestNullStreamAll)
      {
         Stream::NullStream ns;
         Assert::IsTrue(ns.CanRead());
         Assert::IsTrue(ns.CanWrite());
         Assert::IsTrue(ns.CanSeek());

         // test read
         Assert::IsTrue(false == ns.AtEndOfStream());

         BYTE abBuffer[3] = { 1, 2, 3 };
         DWORD dwBytesRead = 0;
         Assert::IsTrue(ns.Read(abBuffer, sizeof(abBuffer), dwBytesRead));
         Assert::IsTrue(sizeof(abBuffer) == dwBytesRead);
         Assert::IsTrue(abBuffer[0] == abBuffer[1] == abBuffer[2] == 0);

         Assert::IsTrue(0 == ns.ReadByte());

         Assert::IsTrue(false == ns.AtEndOfStream());

         // test write
         DWORD dwBytesWritten = 0;
         ns.Write(abBuffer, sizeof(abBuffer), dwBytesWritten);
         Assert::IsTrue(dwBytesWritten == sizeof(abBuffer));
         ns.WriteByte(42);

         // test seek
         Assert::IsTrue(0UL == ns.Seek(0L, Stream::IStream::seekBegin));
         Assert::IsTrue(0UL == ns.Seek(0L, Stream::IStream::seekCurrent));
         Assert::IsTrue(0UL == ns.Seek(0L, Stream::IStream::seekEnd));
         Assert::IsTrue(0UL == ns.Position());
         Assert::IsTrue(0UL == ns.Length());

         ns.Flush();
         ns.Close();
      }
   };

} // namespace UnitTest
