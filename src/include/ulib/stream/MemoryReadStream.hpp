//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2017 Michael Fink
//
/// \file MemoryReadStream.hpp memory read-only stream
//
#pragma once

#include <ulib/config/Common.hpp>
#include <ulib/stream/IStream.hpp>
#include <vector>

namespace Stream
{
   /// read-only memory stream
   class MemoryReadStream : public IStream
   {
   public:
      /// ctor
      MemoryReadStream(const BYTE* data, DWORD_PTR length)
         :m_dataPtr(data),
         m_length(length),
         m_currentPos(0)
      {
      }

      // virtual methods from IStream

      virtual bool CanRead() const override { return true; }
      virtual bool CanWrite() const override { return false; }
      virtual bool CanSeek() const override { return true; }

      virtual bool Read(void* bBuffer, DWORD maxBufferLength, DWORD& numBytesRead) override
      {
         numBytesRead = m_length - m_currentPos > maxBufferLength ? maxBufferLength : static_cast<DWORD>(m_length - m_currentPos);
         memcpy(bBuffer, m_dataPtr + m_currentPos, numBytesRead);
         m_currentPos += numBytesRead;
         return numBytesRead != 0;
      }

      virtual bool AtEndOfStream() const override
      {
         return m_currentPos >= m_length;
      }

      virtual void Write(const void* dataToWrite, DWORD lengthInBytes, DWORD& numBytesWritten) override
      {
         UNUSED(dataToWrite);
         UNUSED(lengthInBytes);
         UNUSED(numBytesWritten);

         ATLASSERT(false); // can't write to stream
      }

      virtual ULONGLONG Seek(LONGLONG offset, ESeekOrigin origin) override
      {
         switch (origin)
         {
         case seekBegin:
            m_currentPos = static_cast<DWORD_PTR>(offset);
            break;

         case seekCurrent:
         {
            LONGLONG llResult = m_currentPos + offset;
            m_currentPos = llResult < 0 ? 0 : static_cast<DWORD_PTR>(llResult);
         }
         break;

         case seekEnd:
            m_currentPos = static_cast<DWORD_PTR>(offset) > m_length ? 0 : m_length - static_cast<DWORD_PTR>(offset);
            break;

         default:
            ATLASSERT(false); // invalid seek origin
            break;
         }

         if (m_currentPos > m_length)
            m_currentPos = m_length;

         return Position();
      }

      virtual ULONGLONG Position() override
      {
         return static_cast<ULONGLONG>(m_currentPos);
      }

      virtual ULONGLONG Length() override
      {
         return static_cast<ULONGLONG>(m_length);
      }

      virtual void Flush() override
      {
         // nothing to do here
      }

      virtual void Close() override
      {
         m_dataPtr = nullptr;
         m_length = m_currentPos = 0;
      }

   private:
      /// data bytes
      const BYTE* m_dataPtr;

      /// length of data
      DWORD_PTR m_length;

      /// current position
      DWORD_PTR m_currentPos;
   };

} // namespace Stream
