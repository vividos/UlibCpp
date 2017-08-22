//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012,2017 Michael Fink
//
/// \file Exception.hpp exception base class
//
#pragma once

#include <exception>

/// exception base class
class Exception : public std::exception
{
public:
   /// ctor
   Exception(LPCSTR sourceFile, UINT sourceLine)
      :std::exception(FormatExceptionText(nullptr, sourceFile, sourceLine)),
      m_sourceFile(sourceFile),
      m_sourceLine(sourceLine)
   {
   }

   /// ctor
   Exception(const CString& message, LPCSTR sourceFile, UINT sourceLine)
      :std::exception(FormatExceptionText(message.GetString(), sourceFile, sourceLine)),
      m_message(message),
      m_sourceFile(sourceFile),
      m_sourceLine(sourceLine)
   {
   }

   /// returns exception message
   CString Message() const { return m_message; }

   /// returns source file where the exception occured
   CString SourceFile() const { return m_sourceFile; }

   /// returns source file line where the exception occured
   UINT SourceLine() const { return m_sourceLine; }

private:
   /// formats exception text; used to pass a text to std::exception() base class ctor
   static CStringA FormatExceptionText(LPCTSTR message, LPCSTR sourceFile, UINT sourceLine)
   {
      CStringA text;
      text.Format(
         "%s(%u): %ls",
         sourceFile,
         sourceLine,
         message == nullptr ? _T("no message given") : message);

      return text;
   }

private:
   /// exception message
   CString m_message;

   /// source file name
   CString m_sourceFile;

   /// source file line
   UINT m_sourceLine;
};
