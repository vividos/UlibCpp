//
// ulib - a collection of useful classes
// Copyright (C) 2007,2017 Michael Fink
//
/// \file TestTextStreamFilter.cpp tests for TextStreamFilter class
//

#include "stdafx.h"
#include <ulib/stream/MemoryReadStream.hpp>
#include <ulib/stream/TextStreamFilter.hpp>
#include <ulib/stream/MemoryStream.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests text stream filter class
   TEST_CLASS(TestTextStreamFilter)
   {
      /// tests reading ansi characters
      TEST_METHOD(TestReadCharAnsi)
      {
         BYTE abData[] = { 0x41, 0x42, 0x62, 0x61 };

         Stream::MemoryReadStream ms(abData, sizeof(abData));

         Stream::TextStreamFilter filter(ms, Stream::TextStreamFilter::textEncodingAnsi);

         Assert::IsTrue(static_cast<TCHAR>(abData[0]) == filter.ReadChar());
         Assert::IsTrue(static_cast<TCHAR>(abData[1]) == filter.ReadChar());
         Assert::IsTrue(static_cast<TCHAR>(abData[2]) == filter.ReadChar());
         Assert::IsTrue(static_cast<TCHAR>(abData[3]) == filter.ReadChar());
      }

      /// tests reading native characters
      TEST_METHOD(TestReadCharNative)
      {
         BYTE abData[] = { 0x41, 0x42, 0x62, 0x61 };

         Stream::MemoryReadStream ms(abData, sizeof(abData));

         Stream::TextStreamFilter filter(ms, Stream::TextStreamFilter::textEncodingNative);

#if defined(UNICODE) || defined(_UNICODE)
         Assert::IsTrue(0x4241 == filter.ReadChar());
         Assert::IsTrue(0x6162 == filter.ReadChar());
#else
         Assert::IsTrue(static_cast<TCHAR>(abData[0]) == filter.ReadChar());
         Assert::IsTrue(static_cast<TCHAR>(abData[1]) == filter.ReadChar());
         Assert::IsTrue(static_cast<TCHAR>(abData[2]) == filter.ReadChar());
         Assert::IsTrue(static_cast<TCHAR>(abData[3]) == filter.ReadChar());
#endif
      }

      /// tests reading UTF8 characters
      TEST_METHOD(TestReadCharUTF8)
      {
         BYTE abData[] = { 0xef, 0xbb, 0xbf, 0x41 };

         Stream::MemoryReadStream ms(abData, sizeof(abData));

         Stream::TextStreamFilter filter(ms, Stream::TextStreamFilter::textEncodingUTF8);

#if defined(UNICODE) || defined(_UNICODE)
         Assert::IsTrue(0xfeff == filter.ReadChar());
         Assert::IsTrue(0x0041 == filter.ReadChar());
#else
         // note: don't know exactly what this returns!
         Assert::IsTrue(0x00 == filter.ReadChar());
         Assert::IsTrue(0x41 == filter.ReadChar());
#endif
      }

      /// tests reading UCS16 characters
      TEST_METHOD(TestReadCharUCS16)
      {
         BYTE abData[] = { 0x42, 0x00, 0x61, 0x00 };

         Stream::MemoryReadStream ms(abData, sizeof(abData));

         Stream::TextStreamFilter filter(ms, Stream::TextStreamFilter::textEncodingUCS2);

         Assert::IsTrue(0x0042 == filter.ReadChar());
         Assert::IsTrue(0x0061 == filter.ReadChar());
      }

      /// character for carriage return
      const TCHAR c_cCR = _T('\r');

      /// character for line feed
      const TCHAR c_cLF = _T('\n');

      LPCTSTR pszLine1 = _T("AB");
      LPCTSTR pszLine2 = _T("CD");

      TCHAR aszDataCR[5] = { _T('A'), _T('B'), c_cCR, _T('C'), _T('D') };
      TCHAR aszDataLF[5] = { _T('A'), _T('B'), c_cLF, _T('C'), _T('D') };
      TCHAR aszDataCRLF[6] = { _T('A'), _T('B'), c_cCR, c_cLF, _T('C'), _T('D') };


      /// test ReadLine, data CR, line mode CR
      TEST_METHOD(TestReadLine1)
      {
         Stream::MemoryReadStream msCR(reinterpret_cast<const BYTE*>(aszDataCR), sizeof(aszDataCR));
         Stream::TextStreamFilter filter(msCR,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCR);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(pszLine1 == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == msCR.AtEndOfStream());
      }

      /// test ReadLine, data LF, line mode LF
      TEST_METHOD(TestReadLine2)
      {
         Stream::MemoryReadStream msLF(reinterpret_cast<const BYTE*>(aszDataLF), sizeof(aszDataLF));
         Stream::TextStreamFilter filter(msLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingLF);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(pszLine1 == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == msLF.AtEndOfStream());
      }

      /// test ReadLine, data CRLF, line mode CRLF
      TEST_METHOD(TestReadLine3)
      {
         Stream::MemoryReadStream msCRLF(reinterpret_cast<const BYTE*>(aszDataCRLF), sizeof(aszDataCRLF));
         Stream::TextStreamFilter filter(msCRLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCRLF);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(pszLine1 == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == msCRLF.AtEndOfStream());
      }

      /// test ReadLine, data CR, line mode any
      TEST_METHOD(TestReadLine4)
      {
         Stream::MemoryReadStream msCR(reinterpret_cast<const BYTE*>(aszDataCR), sizeof(aszDataCR));
         Stream::TextStreamFilter filter(msCR,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingReadAny);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(pszLine1 == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == msCR.AtEndOfStream());
      }

      /// test ReadLine, data LF, line mode any
      TEST_METHOD(TestReadLine5)
      {
         Stream::MemoryReadStream msLF(reinterpret_cast<const BYTE*>(aszDataLF), sizeof(aszDataLF));
         Stream::TextStreamFilter filter(msLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingReadAny);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(pszLine1 == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == msLF.AtEndOfStream());
      }

      /// test ReadLine, data CRLF, line mode any
      TEST_METHOD(TestReadLine6)
      {
         Stream::MemoryReadStream msCRLF(reinterpret_cast<const BYTE*>(aszDataCRLF), sizeof(aszDataCRLF));
         Stream::TextStreamFilter filter(msCRLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingReadAny);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(pszLine1 == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == msCRLF.AtEndOfStream());
      }

      /// test ReadLine, mismatched line mode, data CR, line mode LF
      TEST_METHOD(TestReadLine7)
      {
         Stream::MemoryReadStream msCR(reinterpret_cast<const BYTE*>(aszDataCR), sizeof(aszDataCR));
         Stream::TextStreamFilter filter(msCR,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingLF);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(_T("AB\rCD") == text);

         Assert::IsTrue(true == msCR.AtEndOfStream());
      }

      /// test ReadLine, mismatched line mode, data CR, line mode CRLF
      TEST_METHOD(TestReadLine8)
      {
         Stream::MemoryReadStream msCR(reinterpret_cast<const BYTE*>(aszDataCR), sizeof(aszDataCR));
         Stream::TextStreamFilter filter(msCR,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCRLF);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(_T("AB\rCD") == text);

         Assert::IsTrue(true == msCR.AtEndOfStream());
      }

      /// test ReadLine, mismatched line mode, data LF, line mode CR
      TEST_METHOD(TestReadLine9)
      {
         Stream::MemoryReadStream msLF(reinterpret_cast<const BYTE*>(aszDataLF), sizeof(aszDataLF));
         Stream::TextStreamFilter filter(msLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCR);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(_T("AB\nCD") == text);

         Assert::IsTrue(true == msLF.AtEndOfStream());
      }

      /// test ReadLine, mismatched line mode, data LF, line mode CRLF
      TEST_METHOD(TestReadLine10)
      {
         Stream::MemoryReadStream msLF(reinterpret_cast<const BYTE*>(aszDataLF), sizeof(aszDataLF));
         Stream::TextStreamFilter filter(msLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCRLF);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(_T("AB\nCD") == text);

         Assert::IsTrue(true == msLF.AtEndOfStream());
      }

      /// test ReadLine, mismatched line mode, data CRLF, line mode CR
      TEST_METHOD(TestReadLine11)
      {
         Stream::MemoryReadStream msCRLF(reinterpret_cast<const BYTE*>(aszDataCRLF), sizeof(aszDataCRLF));
         Stream::TextStreamFilter filter(msCRLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCR);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(_T("AB") == text);

         filter.ReadLine(text);
         Assert::IsTrue(_T("\nCD") == text);

         Assert::IsTrue(true == msCRLF.AtEndOfStream());
      }

      /// test ReadLine, mismatched line mode, data CRLF, line mode LF
      TEST_METHOD(TestReadLine12)
      {
         Stream::MemoryReadStream msCRLF(reinterpret_cast<const BYTE*>(aszDataCRLF), sizeof(aszDataCRLF));
         Stream::TextStreamFilter filter(msCRLF,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingLF);

         CString text;
         filter.ReadLine(text);
         Assert::IsTrue(_T("AB\r") == text);

         filter.ReadLine(text);
         Assert::IsTrue(pszLine2 == text);

         Assert::IsTrue(true == filter.AtEndOfStream());
         Assert::IsTrue(true == msCRLF.AtEndOfStream());
      }

      /// tests WriteText(), ANSI encoding
      TEST_METHOD(TestWriteText1)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingAnsi, Stream::TextStreamFilter::lineEndingNative);

         filter.Write(pszLine1);
         filter.Flush();

         Assert::IsTrue(2 * sizeof(char) == ms.GetData().size());
         Assert::IsTrue(static_cast<char>(pszLine1[0]) == reinterpret_cast<LPCSTR>(&ms.GetData()[0])[0]);
         Assert::IsTrue(static_cast<char>(pszLine1[1]) == reinterpret_cast<LPCSTR>(&ms.GetData()[0])[1]);
      }

      /// tests WriteText(), UCS-2 encoding
      TEST_METHOD(TestWriteText2)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingUCS2, Stream::TextStreamFilter::lineEndingNative);

         filter.Write(pszLine1);

         Assert::IsTrue(2 * sizeof(WCHAR) == ms.GetData().size());
         Assert::IsTrue(static_cast<WCHAR>(pszLine1[0]) == reinterpret_cast<LPCWSTR>(&ms.GetData()[0])[0]);
         Assert::IsTrue(static_cast<WCHAR>(pszLine1[1]) == reinterpret_cast<LPCWSTR>(&ms.GetData()[0])[1]);
      }

      /// tests WriteText(), UTF-8 encoding
      TEST_METHOD(TestWriteText3)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingUTF8, Stream::TextStreamFilter::lineEndingNative);

         // note: that only works in unicode, since in ANSI the string to write couldn't be encoded
#if defined(UNICODE) || defined(_UNICODE)
   // write BOM
         filter.Write(_T("a")_T("\xfeff")_T("b"));

         Assert::IsTrue(5 == ms.GetData().size());
         Assert::IsTrue(static_cast<BYTE>('a') == ms.GetData()[0]);
         Assert::IsTrue(0xef == ms.GetData()[1]);
         Assert::IsTrue(0xbb == ms.GetData()[2]);
         Assert::IsTrue(0xbf == ms.GetData()[3]);
         Assert::IsTrue(static_cast<BYTE>('b') == ms.GetData()[4]);
#endif
      }

      /// check WriteEndline(), CR mode
      TEST_METHOD(TestWriteEndline1)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCR);

         filter.WriteEndline();

         Assert::IsTrue(sizeof(TCHAR) == ms.GetData().size());
         Assert::IsTrue(c_cCR == reinterpret_cast<LPCTSTR>(&ms.GetData()[0])[0]);
      }

      /// check WriteEndline(), LF mode
      TEST_METHOD(TestWriteEndline2)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingLF);

         filter.WriteEndline();

         Assert::IsTrue(sizeof(TCHAR) == ms.GetData().size());
         Assert::IsTrue(c_cLF == reinterpret_cast<LPCTSTR>(&ms.GetData()[0])[0]);
      }

      /// check WriteEndline(), CRLF mode
      TEST_METHOD(TestWriteEndline3)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCRLF);

         filter.WriteEndline();

         Assert::IsTrue(2 * sizeof(TCHAR) == ms.GetData().size());
         Assert::IsTrue(c_cCR == reinterpret_cast<LPCTSTR>(&ms.GetData()[0])[0]);
         Assert::IsTrue(c_cLF == reinterpret_cast<LPCTSTR>(&ms.GetData()[0])[1]);
      }

      /// check that WriteText() + WriteEndline() is the same as WriteLine(), CR mode
      TEST_METHOD(TestWriteLine1)
      {
         Stream::MemoryStream ms1;
         Stream::TextStreamFilter filter1(ms1,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCR);

         Stream::MemoryStream ms2;
         Stream::TextStreamFilter filter2(ms2,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCR);

         filter1.Write(pszLine1);
         filter1.WriteEndline();

         filter2.WriteLine(pszLine1);

         Assert::IsTrue(ms1.GetData().size() == ms2.GetData().size());
         Assert::IsTrue(0 == memcmp(&ms1.GetData()[0], &ms2.GetData()[0], ms1.GetData().size()));
      }

      /// check that WriteText() + WriteEndline() is the same as WriteLine(), LF mode
      TEST_METHOD(TestWriteLine2)
      {
         Stream::MemoryStream ms1;
         Stream::TextStreamFilter filter1(ms1,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingLF);

         Stream::MemoryStream ms2;
         Stream::TextStreamFilter filter2(ms2,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingLF);

         filter1.Write(pszLine1);
         filter1.WriteEndline();

         filter2.WriteLine(pszLine1);

         Assert::IsTrue(ms1.GetData().size() == ms2.GetData().size());
         Assert::IsTrue(0 == memcmp(&ms1.GetData()[0], &ms2.GetData()[0], ms1.GetData().size()));
      }

      /// check that WriteText() + WriteEndline() is the same as WriteLine(), CRLF mode
      TEST_METHOD(TestWriteLine3)
      {
         Stream::MemoryStream ms1;
         Stream::TextStreamFilter filter1(ms1,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCRLF);

         Stream::MemoryStream ms2;
         Stream::TextStreamFilter filter2(ms2,
            Stream::TextStreamFilter::textEncodingNative, Stream::TextStreamFilter::lineEndingCRLF);

         filter1.Write(pszLine1);
         filter1.WriteEndline();

         filter2.WriteLine(pszLine1);

         Assert::IsTrue(ms1.GetData().size() == ms2.GetData().size());
         Assert::IsTrue(0 == memcmp(&ms1.GetData()[0], &ms2.GetData()[0], ms1.GetData().size()));
      }

      /// tests reading utf-8 sequences, part 1
      TEST_METHOD(TestReadWriteUTF8Part1)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingUTF8, Stream::TextStreamFilter::lineEndingNative);

         // U+0041 U+2262 U+0391 U+002E "A<NOT IDENTICAL TO><ALPHA>."
         LPCTSTR pszData = _T("A")_T("\x2262")_T("\x0391")_T(".");

         // write test data
         filter.Write(pszData);
         Assert::IsTrue(7 == ms.Length());

         // compare data
         {
            // test data from RFC 3629
            BYTE abData[] =
            {
               0x41, 0xE2, 0x89, 0xA2, 0xCE, 0x91, 0x2E
            };

            Assert::IsTrue(sizeof(abData) == ms.GetData().size());
            Assert::IsTrue(0 == memcmp(abData, &ms.GetData()[0], sizeof(abData)));
         }

         // rewind
         ms.Seek(0LL, Stream::IStream::seekBegin);

         // read test data
         Assert::IsTrue(pszData[0] == filter.ReadChar());
         Assert::IsTrue(pszData[1] == filter.ReadChar());
         Assert::IsTrue(pszData[2] == filter.ReadChar());
         Assert::IsTrue(pszData[3] == filter.ReadChar());
      }

      /// tests reading utf-8 sequences, part 2
      TEST_METHOD(TestReadWriteUTF8Part2)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingUTF8, Stream::TextStreamFilter::lineEndingNative);

         // U+D55C U+AD6D U+C5B4 (Korean "hangugeo", meaning "the Korean language")
         LPCTSTR pszData = _T("\xD55C")_T("\xAD6D")_T("\xC5B4");

         // write test data
         filter.Write(pszData);
         Assert::IsTrue(9 == ms.Length());

         // compare data
         {
            // test data from RFC 3629
            BYTE abData[] =
            {
               0xED, 0x95, 0x9C, 0xEA, 0xB5, 0xAD, 0xEC, 0x96, 0xB4
            };

            Assert::IsTrue(sizeof(abData) == ms.GetData().size());
            Assert::IsTrue(0 == memcmp(abData, &ms.GetData()[0], sizeof(abData)));
         }

         // rewind
         ms.Seek(0LL, Stream::IStream::seekBegin);

         // read test data
         Assert::IsTrue(pszData[0] == filter.ReadChar());
         Assert::IsTrue(pszData[1] == filter.ReadChar());
         Assert::IsTrue(pszData[2] == filter.ReadChar());
      }

      /// tests reading utf-8 sequences, part 3
      TEST_METHOD(TestReadWriteUTF8Part3)
      {
         Stream::MemoryStream ms;
         Stream::TextStreamFilter filter(ms,
            Stream::TextStreamFilter::textEncodingUTF8, Stream::TextStreamFilter::lineEndingNative);

         // U+65E5 U+672C U+8A9E (Japanese "nihongo", meaning "the Japanese language")
         LPCTSTR pszData = _T("\x65E5")_T("\x672C")_T("\x8A9E");

         // write test data
         filter.Write(pszData);
         Assert::IsTrue(9 == ms.Length());

         // compare data
         {
            // test data from RFC 3629
            BYTE abData[] =
            {
               0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E
            };

            Assert::IsTrue(sizeof(abData) == ms.GetData().size());
            Assert::IsTrue(0 == memcmp(abData, &ms.GetData()[0], sizeof(abData)));
         }

         // rewind
         ms.Seek(0LL, Stream::IStream::seekBegin);

         // read test data
         Assert::IsTrue(pszData[0] == filter.ReadChar());
         Assert::IsTrue(pszData[1] == filter.ReadChar());
         Assert::IsTrue(pszData[2] == filter.ReadChar());
      }

      /// tests CanRead() and CanWrite methods
      TEST_METHOD(TestCanReadCanWrite)
      {
         // set up
         BYTE abData[] = { 0x42, 0x61 };
         Stream::MemoryReadStream readOnlyStream(abData, sizeof(abData));
         Stream::MemoryStream readWriteStream(abData, sizeof(abData));

         Stream::TextStreamFilter readOnlyFilter(readOnlyStream, Stream::TextStreamFilter::textEncodingUTF8);
         Stream::TextStreamFilter readWriteFilter(readWriteStream, Stream::TextStreamFilter::textEncodingUTF8);

         // check
         Assert::IsTrue(readOnlyFilter.CanRead(), L"reading must be possible from read-only filter");
         Assert::IsFalse(readOnlyFilter.CanWrite(), L"writing must be impossible from read-only filter");

         Assert::IsTrue(readWriteFilter.CanRead(), L"reading must be possible from read-write filter");
         Assert::IsTrue(readWriteFilter.CanWrite(), L"writing must be possible from read-write filter");
      }
   };

} // namespace UnitTest
