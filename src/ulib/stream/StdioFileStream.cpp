//
// ulib - a collection of useful classes
// Copyright (C) 2015,2022 Michael Fink
//
/// \file StdioFileStream.cpp stdiofile based stream
//
#include "stdafx.h"
#include <ulib/stream/FileStream.hpp>
#include <ulib/stream/StreamException.hpp>
#include <ulib/win32/ErrorMessage.hpp>
#include <cstdio>
#include <cerrno>

using Stream::FileStream;

CString MessageFromErrno(int errorNr)
{
   return Win32::ErrorMessage(static_cast<DWORD>(errorNr)).ToString();
};

/// \exception StreamException thrown when file couldn't be opened
FileStream::FileStream(LPCTSTR filename, EFileMode fileMode, EFileAccess fileAccess, EFileShare fileShare)
   :m_fileAccess(fileAccess),
   m_atEndOfFile(true),
   m_fileLength((ULONGLONG)-1)
{
   ATLASSERT(filename != nullptr);

#if defined(DEBUG) || defined(_DEBUG)
   if (fileMode == modeTruncate) // write access needed in modeTruncate
      ATLASSERT((fileAccess & FileStream::accessWrite) != 0);
#endif

   const char* mode = nullptr;

   if (fileMode == modeCreateNew)
      mode = fileAccess == EFileAccess::accessRead ? "w" : "w+";
   else if (fileMode == modeAppend)
      mode = fileAccess == EFileAccess::accessRead ? "a" : "a+";
   else
   {
      if (fileAccess == EFileAccess::accessRead) mode = "r";
      else if (fileAccess == EFileAccess::accessWrite) mode = "w";
      else if (fileAccess == EFileAccess::accessReadWrite)
      {
         if (fileMode == EFileMode::modeOpen) mode = "r+";
         else if (fileMode == EFileMode::modeOpenOrCreate) mode = "w+";
         else if (fileMode == EFileMode::modeCreate) mode = "w+";
      }
   }

   if (mode == nullptr)
      throw Stream::StreamException(
         _T("Open: Couldn't determin fopen mode from file mode and access"), __FILE__, __LINE__);

   FILE* fd = fopen(filename, mode);
   if (fd == nullptr)
      throw Stream::StreamException(
         MessageFromErrno(errno) + filename, __FILE__, __LINE__);

   m_spHandle = std::shared_ptr<void>(fd, fclose);

   m_atEndOfFile = false;

   if (fileMode == modeAppend)
      Seek(0L, FileStream::seekEnd);
}

/// \exception StreamException thrown when reading fails
bool FileStream::Read(void* buffer, DWORD maxBufferLength, DWORD& numBytesRead)
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanRead());

   FILE* fd = static_cast<FILE*>(m_spHandle.get());

   numBytesRead = fread(buffer, 1, maxBufferLength, fd);

   m_atEndOfFile = feof(fd) != 0;

   bool hasError = ferror(fd) != 0;

   if (hasError)
      throw Stream::StreamException(
         _T("Read: ") + MessageFromErrno(errno), __FILE__, __LINE__);

   return numBytesRead != 0;
}

bool FileStream::AtEndOfStream() const
{
   if (!IsOpen() || m_atEndOfFile)
      return true;

   FILE* fd = static_cast<FILE*>(m_spHandle.get());

   return feof(fd) != 0;
}

/// \exception StreamException thrown when writing fails
void FileStream::Write(const void* buffer, DWORD lengthInBytes, DWORD& numBytesWritten)
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanWrite());

   FILE* fd = static_cast<FILE*>(m_spHandle.get());

   numBytesWritten = fwrite(buffer, 1, lengthInBytes, fd);

   bool hasError = ferror(fd) != 0;

   if (hasError)
      throw Stream::StreamException(
         _T("Write: ") + MessageFromErrno(errno), __FILE__, __LINE__);
}

/// \exception StreamException thrown when setting file position fails
ULONGLONG FileStream::Seek(LONGLONG llOffset, ESeekOrigin origin)
{
   ATLASSERT(m_spHandle.get() != NULL);
   ATLASSERT(true == CanSeek());

   FILE* fd = static_cast<FILE*>(m_spHandle.get());

   int pos = fseek(fd, static_cast<long>(llOffset), static_cast<int>(origin));
   if (pos != 0)
      throw Stream::StreamException(_T("Seek: ") + MessageFromErrno(errno), __FILE__, __LINE__);

   return ftell(fd);
}

/// \exception StreamException thrown when getting position fails
ULONGLONG FileStream::Position()
{
   return Seek(0LL, seekCurrent);
}

/// \exception StreamException thrown when getting file size fails
ULONGLONG FileStream::Length()
{
   ATLASSERT(m_spHandle.get() != nullptr);

   FILE* fd = static_cast<FILE*>(m_spHandle.get());

   if (m_fileLength != (ULONGLONG)-1)
   {
      return m_fileLength;
   }

   long currentPos = ftell(fd);

   m_fileLength = Seek(0LL, ESeekOrigin::seekEnd);

   Seek(currentPos, ESeekOrigin::seekBegin);

   return m_fileLength;
}

/// \exception StreamException thrown when flushing the file fails
void FileStream::Flush()
{
   ATLASSERT(m_spHandle.get() != nullptr);
   ATLASSERT(true == CanWrite());

   FILE* fd = static_cast<FILE*>(m_spHandle.get());
   int iRet = fflush(fd);
   if (iRet != 0)
      throw Stream::StreamException(
         _T("Flush: ") + MessageFromErrno(errno), __FILE__, __LINE__);
}

void FileStream::Close()
{
   ATLASSERT(m_spHandle.get() != NULL);

   m_spHandle.reset();

   m_atEndOfFile = true;
}
