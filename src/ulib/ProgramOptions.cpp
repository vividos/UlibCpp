//
// ulib - a collection of useful classes
// Copyright (C) 2008,2009,2013,2017 Michael Fink
//
/// \file ProgramOptions.cpp program options implementation
//

#include "stdafx.h"
#include "ProgramOptions.hpp"
#include "CommandLineParser.hpp"
#include "Path.hpp"

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, unsigned int numArgs, T_fnOptionHandler fnOptionHandler)
{
   CString longOptionLower(longOption);
   longOptionLower.MakeLower();

   OptionInfo info(shortOptionChars, longOptionLower, helpText, numArgs, fnOptionHandler);
   m_optionsList.push_back(info);
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, T_fnOptionHandlerSingleArg fnOptionHandler)
{
   T_fnOptionHandler fnOptionHandler2 =
      std::bind(&ProgramOptions::CallSingleArgHandler, std::placeholders::_1, fnOptionHandler);

   RegisterOption(shortOptionChars, longOption, helpText, 1, fnOptionHandler2);
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, CString& argStorage)
{
   T_fnOptionHandlerSingleArg fnOptionHandler =
      std::bind(&ProgramOptions::SetStringArgStorage, std::placeholders::_1, std::ref(argStorage));

   RegisterOption(shortOptionChars, longOption, helpText, fnOptionHandler);
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, bool& optionFlag)
{
   T_fnOptionHandlerSingleArg fnOptionHandler =
      std::bind(&ProgramOptions::SetBoolArgStorage, std::ref(optionFlag));

   RegisterOption(shortOptionChars, longOption, helpText, fnOptionHandler);
}

void ProgramOptions::RegisterHelpOption()
{
   T_fnOptionHandler fnOptionHandler = std::bind(&ProgramOptions::OutputHelp, this);

   RegisterOption(_T("h?"), _T("help"), _T("Shows help"), 0, fnOptionHandler);
}

bool ProgramOptions::OutputHelp()
{
   if (!m_fnOptionOutputHandler)
      return true;

   CString appFilename = Path(m_executableFilename).FilenameAndExt();

   CString helpText;
   helpText.Format(
      _T("Syntax: %s <params> <args>\n")
      _T("Options:\n"),
      appFilename.GetString());

   // append all options
   size_t maxOptionListIndex = m_optionsList.size();
   for (size_t optionListIndex = 0; optionListIndex < maxOptionListIndex; optionListIndex++)
   {
      OptionInfo& optInfo = m_optionsList[optionListIndex];

      // add short option chars
      size_t maxShortOptions = optInfo.m_shortOptionChars.GetLength();
      for (size_t shortOptionIndex = 0; shortOptionIndex < maxShortOptions; shortOptionIndex++)
      {
         helpText.AppendFormat(_T(" -%c"), optInfo.m_shortOptionChars[shortOptionIndex]);
      }

      // add long option string
      helpText.AppendFormat(_T(" --%s"), optInfo.m_longOption.GetString());
      helpText += _T("\n   ");

      // help text
      CString temp = optInfo.m_helpText;
      temp.Replace(_T("\n"), _T("\n   ")); // add proper indentation
      helpText += temp + _T("\n");
   }

   m_fnOptionOutputHandler(helpText);

   m_handledHelp = true;

   return true;
}

void ProgramOptions::Parse(int argc, _TCHAR* argv[])
{
   CommandLineParser parser(argc, argv);
   Parse(parser);
}

void ProgramOptions::Parse(LPCTSTR commandLine)
{
   CommandLineParser parser(commandLine);
   Parse(parser);
}

void ProgramOptions::Parse(CommandLineParser& parser)
{
   // must contain at least one param
   ATLVERIFY(true == parser.GetNext(m_executableFilename));

   CString argument;
   while (parser.GetNext(argument))
   {
      ATLASSERT(argument.GetLength() > 0);

      // check for first char
      TCHAR chArg = argument[0];
      if (argument.GetLength() > 1 && (chArg == _T('/') || chArg == _T('-')))
      {
         TCHAR searchOptionChar = 0;
         CString longSearchOption;

         // it's an option, either short or long one
         TCHAR optionChar = argument[1];
         if (chArg == _T('-') && optionChar == _T('-'))
         {
            // we have a unix-style option, long name only
            longSearchOption = argument.Mid(2);
            longSearchOption.MakeLower();
         }
         else
         {
            // we have either /X or -X or option, short name only
            searchOptionChar = optionChar;
         }

         // now search for the proper option
         bool foundOption = false;
         for (size_t optionsIndex = 0, maxOptionsIndex = m_optionsList.size(); optionsIndex < maxOptionsIndex; optionsIndex++)
         {
            OptionInfo& optInfo = m_optionsList[optionsIndex];

            // check long name first, then short name
            if ((!longSearchOption.IsEmpty() && longSearchOption == optInfo.m_longOption) ||
               (searchOptionChar != 0 && CString(searchOptionChar).FindOneOf(optInfo.m_shortOptionChars) != -1))
            {
               // found long or short option
               foundOption = true;

               // get arguments
               std::vector<CString> argsList;
               CString paramArgs;
               for (unsigned int numArgs = 0; numArgs < optInfo.m_numArgs; numArgs++)
               {
                  parser.GetNext(paramArgs);
                  argsList.push_back(paramArgs);
               }

               if (argsList.size() < optInfo.m_numArgs)
               {
                  // too few arguments
                  if (m_fnOptionOutputHandler)
                     m_fnOptionOutputHandler(CString(_T("Too few parameters for option: ") + argument));
                  break;
               }

               ATLASSERT(optInfo.m_fnOptionHandler != NULL);
               bool ret2 = optInfo.m_fnOptionHandler(argsList);

               if (!ret2)
               {
                  if (m_fnOptionOutputHandler)
                     m_fnOptionOutputHandler(CString(_T("Syntax error for option: ") + argument));
               }

               break;
            }
         } // end for

         if (!foundOption)
         {
            // unknown option
            if (m_fnOptionOutputHandler)
               m_fnOptionOutputHandler(CString(_T("Unknown option: ") + argument));
         }
      }
      else
      {
         // no arg; it's a file
         bool handled = false;
         if (m_fnParameterHandler)
            handled = m_fnParameterHandler(argument);

         if (!handled)
         {
            // output: unhandled option
            if (m_fnOptionOutputHandler)
               m_fnOptionOutputHandler(CString(_T("Unknown parameter: ") + argument));
         }
      }
   }
}

void ProgramOptions::OutputConsole(const CString& text)
{
   _tprintf(_T("%s\n"), text.GetString());
}
