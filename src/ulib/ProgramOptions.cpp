//
// ulib - a collection of useful classes
// Copyright (C) 2008,2009,2013,2017,2020 Michael Fink
//
/// \file ProgramOptions.cpp program options implementation
//

#include "stdafx.h"
#include <ulib/ProgramOptions.hpp>
#include <ulib/CommandLineParser.hpp>
#include <ulib/Path.hpp>

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, unsigned int numArgs, T_fnOptionHandler fnOptionHandler)
{
   CString longOptionLower(longOption);
   longOptionLower.MakeLower();

   OptionInfo info(shortOptionChars, longOptionLower, helpText, numArgs, fnOptionHandler);
   m_optionsList.push_back(info);
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption, const CString& helpText,
   T_fnOptionHandlerNoArg fnOptionHandler)
{
   RegisterOption(shortOptionChars, longOption, helpText, 0,
      [fnOptionHandler](auto dummy)
      {
         UNUSED(dummy);
         return fnOptionHandler();
      });
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, T_fnOptionHandlerSingleArg fnOptionHandler)
{
   RegisterOption(shortOptionChars, longOption, helpText, 1,
      [fnOptionHandler](const std::vector<CString>& argsList)
      {
         ATLASSERT(argsList.size() == 1);
         return fnOptionHandler(argsList[0]);
      });
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, CString& argStorage)
{
   RegisterOption(shortOptionChars, longOption, helpText,
      [&argStorage](const CString& argument)
      {
         argStorage = argument;
         return true;
      });
}

void ProgramOptions::RegisterOption(const CString& shortOptionChars, const CString& longOption,
   const CString& helpText, bool& optionFlag)
{
   RegisterOption(shortOptionChars, longOption, helpText, 0,
      [&optionFlag](auto argsList)
      {
         UNUSED(argsList);
         optionFlag = true;
         return true;
      });
}

void ProgramOptions::RegisterHelpOption()
{
   T_fnOptionHandler fnOptionHandler = std::bind(&ProgramOptions::OutputHelp, this);

   RegisterOption(_T("h?"), _T("help"), _T("Shows help"), 0, fnOptionHandler);
}

bool ProgramOptions::OutputHelp()
{
   m_handledHelp = true;

   if (!m_fnOptionOutputHandler)
      return true;

   CString appFilename = Path::FilenameAndExt(m_executableFilename);

   CString helpText;
   helpText.Format(
      _T("Syntax: %s <params> <args>\n")
      _T("Options:\n"),
      appFilename.GetString());

   // append all options
   size_t maxOptionListIndex = m_optionsList.size();
   for (size_t optionListIndex = 0; optionListIndex < maxOptionListIndex; optionListIndex++)
   {
      const OptionInfo& optInfo = m_optionsList[optionListIndex];

      // add short option chars
      int maxShortOptions = optInfo.m_shortOptionChars.GetLength();
      for (int shortOptionIndex = 0; shortOptionIndex < maxShortOptions; shortOptionIndex++)
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

   return true;
}

void ProgramOptions::Parse(int argc, LPCTSTR argv[])
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
            const OptionInfo& optInfo = m_optionsList[optionsIndex];

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
                  if (parser.GetNext(paramArgs))
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
               bool ret = optInfo.m_fnOptionHandler(argsList);

               if (!ret && m_fnOptionOutputHandler)
                  m_fnOptionOutputHandler(CString(_T("Syntax error for option: ") + argument));

               break;
            }
         } // end for

         // unknown option
         if (!foundOption && m_fnOptionOutputHandler)
            m_fnOptionOutputHandler(CString(_T("Unknown option: ") + argument));
      }
      else
      {
         // no arg; it's a file
         bool handled = false;
         if (m_fnParameterHandler)
            handled = m_fnParameterHandler(argument);

         // output: unhandled option
         if (!handled && m_fnOptionOutputHandler)
            m_fnOptionOutputHandler(CString(_T("Unknown parameter: ") + argument));
      }
   }
}

void ProgramOptions::OutputConsole(const CString& text)
{
   _tprintf(_T("%s\n"), text.GetString());
}
