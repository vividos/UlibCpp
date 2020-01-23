//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2017,2020 Michael Fink
//
/// \file NullStream.hpp null stream
//
#pragma once

#include <ulib/stream/IStream.hpp>

namespace Stream
{
   /// null stream that can be read from (returns just zeros), written to and seeked
   class NullStream : public IStream
   {
   public:
      /// ctor
      NullStream() { /* nothing to do here */ }
      virtual bool CanRead() const override { return true; }
      virtual bool CanWrite() const override { return true; }
      virtual bool CanSeek() const override { return true; }
      virtual bool Read(void* buffer, DWORD maxBufferLength, DWORD& numBytesRead) override
      {
         memset(buffer, 0, maxBufferLength);
         numBytesRead = maxBufferLength;
         return true;
      }

      virtual BYTE ReadByte() override { return 0; }
      virtual bool AtEndOfStream() const override { return false; }
      virtual void Write(const void*, DWORD numBytesToWrite, DWORD& numBytesWritten) override
      {
         numBytesWritten = numBytesToWrite;
      }

      virtual void WriteByte(BYTE) override { /* nothing to do here */ }
      virtual ULONGLONG Seek(LONGLONG, IStream::ESeekOrigin) override { return Position(); }
      virtual ULONGLONG Position() override { return 0; }
      virtual ULONGLONG Length() override { return 0; }
      virtual void Flush() override { /* nothing to do here */ }
      virtual void Close() override { /* nothing to do here */ }
   };

} // namespace Stream
