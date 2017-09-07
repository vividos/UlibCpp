//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file ProgramOptions.hpp program options handling
//
#pragma once

#include <vector>
#include <functional>

// forward references
class CommandLineParser;

/// \brief program options class
/// \details supports parsing program options given at command line
class ProgramOptions
{
public:
   /// option handler function; called when option is parsed
   typedef std::function<bool (const std::vector<CString>&)> T_fnOptionHandler;

   /// option handler function for no argument
   typedef std::function<bool ()> T_fnOptionHandlerNoArg;

   /// option handler function for single argument
   typedef std::function<bool (const CString&)> T_fnOptionHandlerSingleArg;

   /// parameter handler
   typedef std::function<bool (const CString&)> T_fnParameterHandler;

   /// handler for text output
   typedef std::function<void (const CString&)> T_fnOptionOutputHandler;

   /// ctor
   ProgramOptions()
      :m_handledHelp(false)
   {
   }

   /// returns if help option was selected, e.g. when program should exit
   bool IsSelectedHelpOption() const { return m_handledHelp; }

   /// registers a handler for arguments without preceding options; most of the time file names
   void RegisterParameterHandler(T_fnParameterHandler fnParameterHandler)
   {
      m_fnParameterHandler = fnParameterHandler;
   }

   /// registers an output handler; needed when options parser has to output something
   void RegisterOutputHandler(T_fnOptionOutputHandler fnOptionOutputHandler)
   {
      m_fnOptionOutputHandler = fnOptionOutputHandler;
   }

   /// registers an option and sets option handler callback function
   void RegisterOption(const CString& shortOptionChars, const CString& longOption, const CString& helpText, unsigned int numArgs,
      T_fnOptionHandler fnOptionHandler);

   /// registers option with handler that takes no arguments
   void RegisterOption(const CString& shortOptionChars, const CString& longOption, const CString& helpText,
      T_fnOptionHandlerNoArg fnOptionHandler);

   /// registers option with handler that takes single argument
   void RegisterOption(const CString& shortOptionChars, const CString& longOption, const CString& helpText, T_fnOptionHandlerSingleArg fnOptionHandler);

   /// registers option that sets string variable; var must be available until Parse() returns
   void RegisterOption(const CString& shortOptionChars, const CString& longOption, const CString& helpText, CString& argStorage);

   /// registers option that sets boolean variable to true; var must be available until Parse() returns
   void RegisterOption(const CString& shortOptionChars, const CString& longOption, const CString& helpText, bool& optionFlag);

   /// parses command line options, C-style
   void Parse(int argc, _TCHAR* argv[]);

   /// parses command line options, Win32-style
   void Parse(LPCTSTR commandLine);

   /// registers general help option /h, /?, --help
   void RegisterHelpOption();

   /// output help (manually)
   bool OutputHelp();

   /// helper function for outputting to console; can directly be used in call to RegisterOutputHandler()
   static void OutputConsole(const CString& text);

private:
   /// parses command line
   void Parse(CommandLineParser& parser);

   /// calls single argument handler
   static bool CallSingleArgHandler(const std::vector<CString>& argsList, T_fnOptionHandlerSingleArg fnHandler)
   {
      ATLASSERT(argsList.size() == 1);
      return fnHandler(argsList[0]);
   }

   /// sets string argument to storage variable
   static bool SetStringArgStorage(const CString& argument, CString& storage)
   {
      storage = argument;
      return true;
   }

   /// sets bool argument to storage variable
   static bool SetBoolArgStorage(bool& storage)
   {
      storage = true;
      return true;
   }

private:
   /// info about a single option
   struct OptionInfo
   {
      /// ctor
      OptionInfo(const CString& shortOptionChars,
         const CString& longOption,
         const CString& helpText,
         unsigned int numArgs,
         T_fnOptionHandler fnOptionHandler)
         :m_shortOptionChars(shortOptionChars),
          m_longOption(longOption),
          m_helpText(helpText),
         m_numArgs(numArgs),
         m_fnOptionHandler(fnOptionHandler)
      {
      }

      CString m_shortOptionChars;   ///< collection of short option characters for this option
      CString m_longOption;         ///< text of long option
      CString m_helpText;           ///< help text
      unsigned int m_numArgs;       ///< number of args for this option

      /// option handler to call
      T_fnOptionHandler m_fnOptionHandler;
   };

   /// holds the program executable (first argument of command line)
   CString m_executableFilename;

   /// contains all options
   std::vector<OptionInfo> m_optionsList;

   /// function pointer to parameter handler
   T_fnParameterHandler m_fnParameterHandler;

   /// function pointer to option output handler
   T_fnOptionOutputHandler m_fnOptionOutputHandler;

   /// indicates if help option was handled
   bool m_handledHelp;
};
