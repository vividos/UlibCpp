//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2014,2017,2020 Michael Fink
//
/// \file TextFileStream.hpp text file stream
//
#pragma once

// needed includes
#include <ulib/stream/FileStream.hpp>
#include <ulib/stream/TextStreamFilter.hpp>

namespace Stream
{
   /// text file stream
   class TextFileStream : public TextStreamFilter
   {
   public:
      /// ctor; opens or creates text file stream
      TextFileStream(LPCTSTR filename,
         FileStream::EFileMode fileMode,
         FileStream::EFileAccess fileAccess,
         FileStream::EFileShare fileShare,
         ITextStream::ETextEncoding textEncoding = textEncodingNative,
         ITextStream::ELineEndingMode lineEndingMode = lineEndingCRLF)
         :TextStreamFilter(m_fileStream, textEncoding, lineEndingMode),
         m_fileStream(filename, fileMode, fileAccess, fileShare)
      {
      }

      /// special unicode characters
      enum ESpecialChars
      {
         BOM = 0xfeff,  ///< byte order mark U+FEFF (zero width no-break space)
      };

      /// returns if the file was successfully opened
      bool IsOpen() const { return m_fileStream.IsOpen(); }
      /// returns true when the file end is reached
      virtual bool AtEndOfStream() const override { return m_fileStream.AtEndOfStream(); }

   private:
      /// file stream
      FileStream m_fileStream;
   };

} // namespace Stream
