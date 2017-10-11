//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2012,2014,2017 Michael Fink
//
/// \file ITextStream.hpp text stream interface
//
#pragma once

namespace Stream
{
   /// text stream interface
   class ITextStream
   {
   public:
      /// text encoding that is possible for text files
      enum ETextEncoding
      {
         textEncodingNative,  ///< native encoding; compiler options decide if ansi or unicode is used for output
         textEncodingAnsi,    ///< ansi text encoding; depends on the current codepage (not recommended)
         textEncodingUTF8,    ///< UTF-8 encoding
         textEncodingUCS2,    ///< UCS-2 encoding
      };

      /// line ending mode used to detect lines or for writing
      enum ELineEndingMode
      {
         lineEndingCRLF,   ///< a CR and LF char (\\r\\n) is used to separate lines; Win32-style
         lineEndingLF,     ///< a LF char (\\n) is used to separate lines; Linux-style
         lineEndingCR,     ///< a CR char (\\r) is used to separate lines; MAC-style
         lineEndingReadAny,///< when reading, any of the above line ending modes are detected when using ReadLine()
         lineEndingNative, ///< native mode is used
      };

      /// ctor
      ITextStream(ETextEncoding textEncoding = textEncodingNative,
         ELineEndingMode lineEndingMode = lineEndingNative)
         :m_textEncoding(textEncoding),
         m_lineEndingMode(lineEndingMode)
      {
      }

      /// dtor
      virtual ~ITextStream()
      {
         // nothing to cleanup
      }

      // stream capabilities

      /// returns text encoding currently in use
      ETextEncoding TextEncoding() const { return m_textEncoding; }

      /// returns line ending mode currently in use
      ELineEndingMode LineEndingMode() const { return m_lineEndingMode; }

      /// returns true when stream can be read
      virtual bool CanRead() const = 0;

      /// returns true when stream can be written to
      virtual bool CanWrite() const = 0;

      /// returns true when the stream end is reached
      virtual bool AtEndOfStream() const = 0;

      // read support

      /// reads a single character
      virtual TCHAR ReadChar() = 0;

      /// reads a whole line using line ending settings
      virtual void ReadLine(CString& line) = 0;

      // write support

      /// writes text
      virtual void Write(const CString& text) = 0;

      /// writes endline character
      virtual void WriteEndline() = 0;

      /// writes a line
      void WriteLine(const CString& line)
      {
         Write(line);
         WriteEndline();
      }

      /// flushes out text stream
      virtual void Flush() = 0;

   private:
      friend class TextStreamFilter;

      /// current text encoding
      ETextEncoding m_textEncoding;

      /// current line ending mode
      ELineEndingMode m_lineEndingMode;
   };

} // namespace Stream
