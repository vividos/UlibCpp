//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2014,2017 Michael Fink
//
/// \file IStream.hpp stream interface
//
#pragma once

/// \brief stream related classes
namespace Stream
{
   /// stream interface
   class IStream
   {
   public:
      /// origin for Seek() operations
      enum ESeekOrigin
      {
         seekBegin = 0,
         seekCurrent = 1,
         seekEnd = 2,
      };

      /// dtor
      virtual ~IStream() {}

      // stream capabilities

      /// returns true when stream can be read
      virtual bool CanRead() const = 0;
      /// returns true when stream can be written to
      virtual bool CanWrite() const = 0;
      /// returns true when seek operations are possible in the stream
      virtual bool CanSeek() const = 0;

      // read support

      /// reads amount of data into given buffer; returns if stream is at its end
      virtual bool Read(void* buffer, DWORD maxBufferLength, DWORD& numBytesRead) = 0;
      /// reads one byte
      virtual BYTE ReadByte();

      /// returns true when the stream end is reached
      virtual bool AtEndOfStream() const = 0;

      // write support

      /// writes out given data buffer
      virtual void Write(const void* dataToWrite, DWORD lengthInBytes, DWORD& numBytesWritten) = 0;
      /// writes out single byte
      virtual void WriteByte(BYTE byteToWrite);

      // seek support

      /// seeks to given position, regarding given origin
      virtual ULONGLONG Seek(LONGLONG seekOffset, ESeekOrigin origin) = 0;
      /// returns current position in stream
      virtual ULONGLONG Position() = 0;
      /// returns length of stream
      virtual ULONGLONG Length() = 0;

      /// flushes data
      virtual void Flush() = 0;
      /// closes stream
      virtual void Close() = 0;
   };

   inline BYTE IStream::ReadByte()
   {
      BYTE byteToRead;
      DWORD numBytesRead = 0;
      ATLVERIFY(true == Read(&byteToRead, 1, numBytesRead) && 1 == numBytesRead);
      return byteToRead;
   }

   inline void IStream::WriteByte(BYTE byteToWrite)
   {
      DWORD numBytesWritten;
      Write(&byteToWrite, 1, numBytesWritten);
      ATLVERIFY(1 == numBytesWritten);
   }

} // namespace Stream
