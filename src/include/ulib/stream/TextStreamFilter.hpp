//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2014,2017,2020 Michael Fink
//
/// \file TextStreamFilter.hpp text stream filter
//
#pragma once

// needed includes
#include <ulib/stream/IStream.hpp>
#include <ulib/stream/ITextStream.hpp>

namespace Stream
{
   /// text stream filter
   class TextStreamFilter : public ITextStream
   {
   public:
      /// creates or opens text file stream
      TextStreamFilter(IStream& stream,
         ETextEncoding textEncoding = textEncodingNative,
         ELineEndingMode lineEndingMode = lineEndingNative);

      /// reads a single character
      virtual TCHAR ReadChar() override;

      /// reads a whole line
      virtual void ReadLine(CString& line) override;

      /// writes text
      virtual void Write(const CString& text) override;

      /// writes endline character
      virtual void WriteEndline() override;

      /// returns underlying stream (const version)
      const IStream& Stream() const { return m_stream; }

      /// returns underlying stream
      IStream& Stream() { return m_stream; }

      /// returns true when stream can be read
      virtual bool CanRead() const override { return m_stream.CanRead(); }

      /// returns true when stream can be written to
      virtual bool CanWrite() const override { return m_stream.CanWrite(); }

      /// returns true when the stream end is reached
      virtual bool AtEndOfStream() const override { return m_stream.AtEndOfStream(); }

      /// flushes out text stream
      virtual void Flush() override { m_stream.Flush(); }

   private:
      /// puts back one character
      void PutBackChar(TCHAR ch);

   private:
      /// stream to read from / write to
      IStream& m_stream;

      /// indicates if a character was put back
      bool m_isCharPutBack;

      /// the put back character
      TCHAR m_putBackChar;
   };

} // namespace Stream
