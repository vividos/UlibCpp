//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2014,2017 Michael Fink
//
/// \file FileStream.hpp file based stream
//
#pragma once

// needed includes
#include <ulib/stream/IStream.hpp>
#include <memory>

namespace Stream
{
   /// file stream
   class FileStream : public IStream
   {
   public:
      /// file open mode
      enum EFileMode
      {
         modeCreateNew = 1,   ///< creates a new file; fails when it already exists
         modeCreate = 2,      ///< create a new file; if it already exists, it is overwritten
         modeOpen = 3,        ///< open an existing file; fail when no file was found
         modeOpenOrCreate = 4,///< open an existing file; create empty file when no file was found
         modeTruncate = 5,    ///< open an existing file; truncate file to 0 bytes; fail when no file was found
         modeAppend = 6,      ///< like modeOpen, and seeks to the end of the file; no seek before end is allowed
      };

      /// file access
      enum EFileAccess
      {
         accessRead = 0x80000000,      ///< read access to file only
         accessWrite = 0x40000000,     ///< write access to file only
         accessReadWrite = 0xC0000000, ///< read and write access to file
      };

      /// file share mode
      enum EFileShare
      {
         shareNone = 0,       ///< declines reading or writing the file
         shareRead = 1,       ///< allows reading from the file
         shareWrite = 2,      ///< allows writing to the file
         shareReadWrite = 3,  ///< allows both reading and writing
         shareDelete = 4,     ///< file can be deleted by others
      };

      /// ctor; opens or creates a file
      FileStream(LPCTSTR filename, EFileMode fileMode, EFileAccess fileAccess, EFileShare fileShare);

      /// returns if the file was successfully opened
      bool IsOpen() const { return m_spHandle.get() != nullptr; }

      // virtual methods from IStream

      /// returns if the stream can be read (true when opened with accessRead)
      virtual bool CanRead() const { return (m_fileAccess & accessRead) != 0; }
      /// returns if the stream can be written (true when opened with accessWrite)
      virtual bool CanWrite() const { return (m_fileAccess & accessWrite) != 0; }
      /// returns if the stream can be seeked (always true)
      virtual bool CanSeek() const { return true; }

      // read support
      virtual bool Read(void* buffer, DWORD maxBufferLength, DWORD& numBytesRead);
      virtual bool AtEndOfStream() const;

      // write support
      virtual void Write(const void* dataToWrite, DWORD lengthInBytes, DWORD& numBytesWritten);

      // seek support
      virtual ULONGLONG Seek(LONGLONG seekOffset, ESeekOrigin origin);
      virtual ULONGLONG Position();
      virtual ULONGLONG Length();

      virtual void Flush();
      virtual void Close();

   private:
      /// file access mode
      EFileAccess m_fileAccess;

      /// handle to file
      std::shared_ptr<void> m_spHandle;

      /// indicates if end of file is reached
      bool m_atEndOfFile;

      /// file length
      mutable ULONGLONG m_fileLength;
   };

} // namespace Stream
