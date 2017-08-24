//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2012,2014,2017 Michael Fink
//
/// \file FileStream.cpp file based stream
//
#include "stdafx.h"
#include <ulib/stream/FileStream.hpp>
#include <ulib/stream/StreamException.hpp>
#include <ulib/Win32/ErrorMessage.hpp>

using Stream::FileStream;

/// \note when a file on a floppy or cdrom drive is tried to open without a disc in the drive,
///       a message box appears asking for a disc in the drive. Use SetErrorMode with flag
///       SEM_FAILCRITICALERRORS to prevent this.
/// \exception StreamException thrown when file couldn't be opened
FileStream::FileStream(LPCTSTR filename, EFileMode fileMode, EFileAccess fileAccess, EFileShare fileShare)
   :m_fileAccess(fileAccess),
   m_atEndOfFile(true)
{
   ATLASSERT(filename != NULL);

#if defined(DEBUG) || defined(_DEBUG)
   if (fileMode == modeTruncate) // write access needed in modeTruncate
      ATLASSERT((fileAccess & FileStream::accessWrite) != 0);
#endif

   HANDLE fileHandle = CreateFile(filename,
      static_cast<DWORD>(fileAccess), // desired access
      static_cast<DWORD>(fileShare), // share mode
      NULL, // security attributes
      static_cast<DWORD>(fileMode == modeAppend ? modeOpen : fileMode),
      FILE_ATTRIBUTE_NORMAL,
      NULL); // template file handle

   if (fileHandle == INVALID_HANDLE_VALUE)
      throw Stream::StreamException(Win32::ErrorMessage().ToString() + filename, __FILE__, __LINE__);

   m_spHandle = std::shared_ptr<void>(fileHandle, CloseHandle);

   m_atEndOfFile = false;

   if (fileMode == modeAppend)
      Seek(0L, FileStream::seekEnd);
}

/// \exception StreamException thrown when reading fails
bool FileStream::Read(void* buffer, DWORD maxBufferLength, DWORD& numBytesRead)
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanRead());

   numBytesRead = 0;
   BOOL ret = ::ReadFile(m_spHandle.get(), buffer, maxBufferLength, &numBytesRead, NULL);

   if (ret == FALSE)
      throw Stream::StreamException(_T("Read: ") + Win32::ErrorMessage().ToString(), __FILE__, __LINE__);

   return numBytesRead != 0;
}

bool FileStream::AtEndOfStream() const
{
   if (!IsOpen() || m_atEndOfFile)
      return true;

   try
   {
      FileStream& rThis = const_cast<FileStream&>(*this);

      // find out current file pointer
      ULONGLONG currentPos = rThis.Seek(0LL, Stream::IStream::seekCurrent);
      ULONGLONG endPos = rThis.Seek(0LL, Stream::IStream::seekEnd);

      if (currentPos == endPos)
         return true;

      // not at EOF, so seek back
      rThis.Seek(static_cast<LONGLONG>(currentPos), Stream::IStream::seekBegin);
   }
   catch (const Stream::StreamException&)
   {
      // exception while seek: possibly on end of stream
      return true;
   }

   return false;
}

/// \exception StreamException thrown when writing fails
void FileStream::Write(const void* dataToWrite, DWORD lengthInBytes, DWORD& numBytesWritten)
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanWrite());

   numBytesWritten = 0;
   BOOL ret = ::WriteFile(m_spHandle.get(), dataToWrite,
      lengthInBytes, &numBytesWritten, NULL);

   if (ret == FALSE)
      throw Stream::StreamException(_T("Write: ") + Win32::ErrorMessage().ToString(), __FILE__, __LINE__);
}

/// \exception StreamException thrown when setting file position fails
ULONGLONG FileStream::Seek(LONGLONG seekOffset, ESeekOrigin origin)
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanSeek());

   LARGE_INTEGER li;
   li.QuadPart = seekOffset;

   li.LowPart = ::SetFilePointer(m_spHandle.get(),
      li.LowPart,
      &li.HighPart,
      static_cast<DWORD>(origin));

   // win32 may have returned error, or the low part just was 0xffffffff
   if (li.LowPart == INVALID_SET_FILE_POINTER)
   {
      // ... so also check last error
      DWORD dwLastError = GetLastError();
      if (dwLastError != ERROR_SUCCESS)
         throw Stream::StreamException(_T("Seek: ") + Win32::ErrorMessage().ToString(), __FILE__, __LINE__);
   }

   return li.QuadPart;
}

/// \exception StreamException thrown when getting position fails
ULONGLONG FileStream::Position()
{
   return Seek(0L, seekCurrent);
}

/// \exception StreamException thrown when getting file size fails
ULONGLONG FileStream::Length()
{
   ATLASSERT(m_spHandle.get() != NULL);

#ifdef _WIN32_WCE
   // CE has no GetFileSizeEx, so we have to use GetFileSize
   LARGE_INTEGER fileSize; fileSize.QuadPart = 0;
   DWORD dwRet = ::GetFileSize(m_spHandle.get(), reinterpret_cast<ULONG*>(&fileSize.HighPart));
   if (dwRet == 0xffffffff)
   {
      DWORD dwError = GetLastError();

      if (dwError != NO_ERROR)
         throw Stream::StreamException(_T("Length: ") + Tools::CWin32ErrorMessage(dwError).GetMessage(), __FILE__, __LINE__);
   }
#else
   LARGE_INTEGER fileSize; fileSize.QuadPart = 0;
   BOOL ret = ::GetFileSizeEx(m_spHandle.get(), &fileSize);

   if (ret == FALSE)
      throw Stream::StreamException(_T("Length: ") + Win32::ErrorMessage().ToString(), __FILE__, __LINE__);
#endif

   return fileSize.QuadPart;
}

/// \exception StreamException thrown when flushing the file fails
void FileStream::Flush()
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanWrite());

   BOOL ret = ::FlushFileBuffers(m_spHandle.get());

   if (ret == FALSE)
      throw Stream::StreamException(_T("Flush: ") + Win32::ErrorMessage().ToString(), __FILE__, __LINE__);
}

void FileStream::Close()
{
   ATLASSERT(m_spHandle.get() != NULL);

   m_spHandle.reset();

   m_atEndOfFile = true;
}
