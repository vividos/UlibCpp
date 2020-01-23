//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2014,2017 Michael Fink
//
/// \file MemoryStream.hpp memory read-write stream
//
#pragma once

// needed includes
#include <ulib/stream/IStream.hpp>
#include <vector>

namespace Stream
{
   /// read-write memory stream
   class MemoryStream : public IStream
   {
   public:
      /// ctor; opens an empty memory stream
      MemoryStream()
         :m_currentPos(0)
      {
      }

      /// ctor; provides memory contents for memory stream
      MemoryStream(const BYTE* dataToUse, DWORD_PTR lengthInBytes)
         :m_currentPos(0)
      {
         m_memoryData.assign(dataToUse, dataToUse + lengthInBytes);
      }

      /// returns data
      const std::vector<BYTE>& GetData() const { return m_memoryData; }

      // virtual methods from IStream

      virtual bool CanRead() const { return true; }
      virtual bool CanWrite() const { return true; }
      virtual bool CanSeek() const { return true; }

      virtual bool Read(void* buffer, DWORD maxBufferLength, DWORD& numBytesRead)
      {
         numBytesRead = m_memoryData.size() - m_currentPos > maxBufferLength ? maxBufferLength :
            static_cast<DWORD>(m_memoryData.size() - m_currentPos);
         if (numBytesRead > 0)
         {
            memcpy(buffer, &m_memoryData[m_currentPos], numBytesRead);
            m_currentPos += numBytesRead;
         }
         return numBytesRead != 0;
      }

      virtual bool AtEndOfStream() const { return m_currentPos >= m_memoryData.size(); }

      /// \exception std::exception when resizing vector fails
      virtual void Write(const void* dataToWrite, DWORD lengthInBytes, DWORD& numBytesWritten)
      {
         // add at current pos
         if (m_memoryData.size() < m_currentPos + lengthInBytes)
            m_memoryData.resize(m_currentPos + lengthInBytes);

         memcpy(&m_memoryData[m_currentPos], dataToWrite, lengthInBytes);
         numBytesWritten = lengthInBytes;
         m_currentPos += numBytesWritten;
      }

      virtual ULONGLONG Seek(LONGLONG seekOffset, ESeekOrigin origin)
      {
         switch (origin)
         {
         case seekBegin:
            m_currentPos = static_cast<size_t>(seekOffset);
            break;

         case seekCurrent:
         {
            LONGLONG resultPosition = m_currentPos + seekOffset;
            m_currentPos = resultPosition < 0 ? 0 : static_cast<size_t>(resultPosition);
         }
         break;

         case seekEnd:
            m_currentPos = static_cast<size_t>(seekOffset) > m_memoryData.size() ? 0 : m_memoryData.size() - static_cast<size_t>(seekOffset);
            break;

         default:
            ATLASSERT(false); // invalid seek origin
            break;
         }

         if (m_currentPos > m_memoryData.size())
            m_currentPos = m_memoryData.size();

         return Position();
      }
      virtual ULONGLONG Position() { return static_cast<ULONGLONG>(m_currentPos); }
      virtual ULONGLONG Length() { return static_cast<ULONGLONG>(m_memoryData.size()); }

      virtual void Flush()
      {
         // nothing to do for memory stream
      }

      virtual void Close()
      {
         m_memoryData.clear();
      }

   private:
      /// data bytes
      std::vector<BYTE> m_memoryData;

      /// current position
      size_t m_currentPos;
   };

} // namespace Stream
