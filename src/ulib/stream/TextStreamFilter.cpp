//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2014,2017 Michael Fink
//
/// \file TextStreamFilter.cpp text stream filter
//
#include "stdafx.h"
#include <ulib/stream/TextStreamFilter.hpp>
#include <ulib/Exception.hpp>
#include <ulib/UTF8.hpp>
#include <vector>

/// character for carriage return
const TCHAR c_cCR = _T('\r');

/// character for line feed
const TCHAR c_cLF = _T('\n');

using Stream::TextStreamFilter;

TextStreamFilter::TextStreamFilter(Stream::IStream& stream,
   ETextEncoding textEncoding,
   ELineEndingMode lineEndingMode)
   :ITextStream(textEncoding, lineEndingMode),
   m_stream(stream),
   m_isCharPutBack(false),
   m_putBackChar(0)
{
   if (textEncoding == textEncodingNative)
#if defined(_UNICODE) || defined(UNICODE)
      m_textEncoding = textEncodingUCS2;
#else
      m_textEncoding = textEncodingAnsi;
#endif

   if (lineEndingMode == lineEndingNative)
#ifdef _WIN32
      m_lineEndingMode = lineEndingCRLF;
#elif defined(__ANDROID__)
      m_lineEndingMode = lineEndingLF;
#else
#  error define proper line ending mode for this platform!
#endif
}

TCHAR TextStreamFilter::ReadChar()
{
   if (m_isCharPutBack)
   {
      m_isCharPutBack = false;
      return m_putBackChar;
   }

   TCHAR ch = 0;

   // depending on the text encoding type, read next byte
   switch (m_textEncoding)
   {
   case textEncodingNative:
      // note: textEncodingNative should have been set to the appropriate encoding in ctor!
      ATLASSERT(false);
      break;

   case textEncodingAnsi:
   {
      char chAnsi = m_stream.ReadByte();

      // use CString convert
      CString cszCh(chAnsi);
      ch = cszCh.GetAt(0);
   }
   break;

   case textEncodingUTF8:
   {
      // read first octet; determines how much further octets are needed; all
      // remaining octets start with bit 7 set and bit 6 cleared (10xx xxxx)
      BYTE bLeading = m_stream.ReadByte();
      if (bLeading <= 0x7f)
      {
         // easy case: char in ASCII zone
         ch = static_cast<char>(bLeading);
         break;
      }

      // count bits from bit 7 until a 0 occurs
      unsigned int uiCount = 0;
      while ((bLeading & 0x80) != 0)
         uiCount++, bLeading <<= 1;

      // only one bit? or more than 6? illegal value for leading octet!
      if (uiCount <= 1 || uiCount > 6)
         throw Exception(_T("illegal utf8 lead byte encountered"), __FILE__, __LINE__);

      // move remaining bits in position
      bLeading >>= uiCount;

      // move remaining bits to bits 6..n, so that the next byte's 6 bits
      // can fill out the dword
      DWORD dwBits = bLeading;

      // read in remaining bytes, remove bits 6 and 7 and add it to the resulting byte
      for (unsigned int ui = 1; ui < uiCount; ui++)
      {
         BYTE bNext = m_stream.ReadByte();
         if ((bNext & 0xc0) != 0x80)
            throw Exception(_T("illegal utf8 byte encountered"), __FILE__, __LINE__);
         dwBits <<= 6;
         dwBits |= bNext & 0x3f;
      }

      if (dwBits > 0xffff)
         throw Exception(_T("utf8 character out of range"), __FILE__, __LINE__);

#if defined(_UNICODE) || defined(UNICODE)
      ch = static_cast<TCHAR>(dwBits & 0xffff);
#else
      WCHAR chw = static_cast<WCHAR>(dwBits & 0xffff);

      CStringA ansiChar(chw);
      ch = ansiChar.GetAt(0);
#endif
   }
   break;

   case textEncodingUCS2:
      // assume UCS2-LE
      ch = m_stream.ReadByte();
      ch |= static_cast<TCHAR>(m_stream.ReadByte()) << 8;
      break;

   default:
      ATLASSERT(false);
      break;
   }

   return ch;
}

void TextStreamFilter::ReadLine(CString& line)
{
   line.Empty();

   TCHAR lastCharacter = 0;
#ifdef _WIN32_WCE
   ULONGLONG position = 0UL;
#else
   ULONGLONG position = 0ULL;
#endif

   // depending on the line type, read in a line of characters
   for (; !m_stream.AtEndOfStream();)
   {
      TCHAR ch = ReadChar();

      // carriage return: can be CRLF, CR, and "any"
      if (ch == c_cCR)
      {
         if (m_lineEndingMode == lineEndingCR)
            break; // found CR; in ReadAny mode a LF might follow

         if (m_lineEndingMode == lineEndingReadAny)
         {
            // found CR; a a LF might follow; read more chars
            lastCharacter = ch;
            position = m_stream.Position();
            continue;
         }

         if (m_lineEndingMode == lineEndingCRLF)
         {
            // check if at end of stream
            if (m_stream.AtEndOfStream())
               break;

            // threre might be a LF, but we don't know for sure, so check
            TCHAR nextCh = ReadChar();
            if (c_cLF == nextCh)
               break; // recognized CRLF

            // some other char, just do normal processing by appending CR, and
            // putting back character
            PutBackChar(nextCh);
         }
      }
      else
         // line feed: can be LF, and the LF part in CRLF when in "any" mode
         if (ch == c_cLF)
         {
            if (m_lineEndingMode == lineEndingLF)
               break; // found LF

            // in any mode, the last character must be a CR to start a new line
            if (m_lineEndingMode == lineEndingReadAny)
            {
               // last character may have been a CR, so we got CRLF
               // or it was another character, then we simply got LF
               break;
            }
         }
         else
            // not CR nor LF, but previous was a CR
            if (m_lineEndingMode == lineEndingReadAny && lastCharacter == c_cCR)
            {
               // recognized a CR line ending; try to put back current
               if (m_stream.CanSeek())
               {
                  // seek back to previous position
                  m_stream.Seek(position, Stream::IStream::seekBegin);
               }
               else
               {
                  // can't seek, so put back character
                  PutBackChar(ch);
               }
               break; // line is finished
            }

      line += ch;

      if (m_lineEndingMode == lineEndingReadAny)
         lastCharacter = ch;
   }
}

void TextStreamFilter::Write(const CString& text)
{
   // write text in proper encoding
   DWORD numWriteBytes = 0;

   switch (m_textEncoding)
   {
   case textEncodingAnsi:
   {
      CStringA ansiText = text;
      LPCSTR textPtr = ansiText.GetString();
      if (ansiText.GetLength() > 0 && textPtr != nullptr)
         m_stream.Write(textPtr, static_cast<DWORD>(strlen(textPtr) * sizeof(*textPtr)), numWriteBytes);
   }
   break;

   case textEncodingUTF8:
   {
      std::vector<char> buffer;
      StringToUTF8(text, buffer);

      // don't write null byte at the end
      m_stream.Write(buffer.data(), static_cast<DWORD>(buffer.size() - 1), numWriteBytes);
   }
   break;

   case textEncodingUCS2:
   {
      CStringW unicodeText = text;
      LPCWSTR textPtr = unicodeText;
      m_stream.Write(textPtr, static_cast<DWORD>(wcslen(textPtr) * sizeof(*textPtr)), numWriteBytes);
   }
   break;

   default:
      ATLASSERT(false);
      break;
   }
}

void TextStreamFilter::WriteEndline()
{
   ATLASSERT(m_lineEndingMode != lineEndingReadAny);

   switch (m_lineEndingMode)
   {
   case lineEndingCRLF: Write(_T("\r\n")); break;
   case lineEndingLF:   Write(_T("\n")); break;
   case lineEndingCR:   Write(_T("\r")); break;

   default:
      ATLASSERT(false);
      break;
   }
}

void TextStreamFilter::PutBackChar(TCHAR ch)
{
   ATLASSERT(false == m_isCharPutBack);

   m_putBackChar = ch;
   m_isCharPutBack = true;
}
