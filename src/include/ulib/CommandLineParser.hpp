//
// ulib - a collection of useful classes
// Copyright (C) 2008,2009,2012,2013,2017 Michael Fink
//
/// \file CommandLineParser.hpp command line parser
//
#pragma once

/// \brief command line parser
/// \details parses command lines; supports double-quoted parameters
class CommandLineParser
{
public:
   /// parses single long command line string
   explicit CommandLineParser(const CString& commandLine = GetCommandLine());

   /// parses _tmain parameter
   CommandLineParser(int argc, TCHAR* argv[]);

   /// returns next parameter
   bool GetNext(CString& nextParameter);

private:
   /// command line
   CString m_commandLine;

   /// current index into line
   int m_currentIndex;
};
