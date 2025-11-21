//
// ulib - a collection of useful classes
// Copyright (C) 2008,2009,2013,2017 Michael Fink
//
/// \file CommandLineParser.cpp command line parser
//

#include "stdafx.h"
#include <ulib/CommandLineParser.hpp>

CommandLineParser::CommandLineParser(const CString& commandLine)
   :m_commandLine(commandLine),
   m_currentIndex(0)
{
}

CommandLineParser::CommandLineParser(int argc, LPCTSTR argv[])
   : m_currentIndex(0)
{
   for (int argIndex = 0; argIndex < argc; argIndex++)
   {
      m_commandLine += argv[argIndex];
      if (argIndex < argc - 1)
         m_commandLine += _T(" ");
   }
}

bool CommandLineParser::GetNext(CString& nextParameter)
{
   if (m_commandLine.IsEmpty() || m_currentIndex >= m_commandLine.GetLength())
      return false;

   // find out next stopper
   TCHAR stopperChar = _T(' ');
   if (m_commandLine[m_currentIndex] == _T('\"'))
   {
      m_currentIndex++;
      stopperChar = _T('\"');
   }

   // search for stopper
   int maxIndex = m_commandLine.Find(stopperChar, m_currentIndex);

   // extract next parameter and advance index
   if (maxIndex == -1)
   {
      nextParameter = m_commandLine.Mid(m_currentIndex);
      m_currentIndex = m_commandLine.GetLength();
   }
   else
   {
      nextParameter = m_commandLine.Mid(m_currentIndex, maxIndex - m_currentIndex);
      m_currentIndex = maxIndex + 1;
   }

   // trim " stopper
   if (stopperChar != _T(' '))
   {
      nextParameter.Trim(_T("\""));
   }

   // eat space chars
   while (m_currentIndex < m_commandLine.GetLength() &&
      m_commandLine[m_currentIndex] == ' ')
      m_currentIndex++;

   return !m_commandLine.IsEmpty();
}
