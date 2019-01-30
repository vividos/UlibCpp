//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file PatternLayout.cpp pattern layout implementation
//
#include "stdafx.h"
#include <ulib/log/PatternLayout.hpp>

void Log::PatternLayout::Format(CString& outputText, const LoggingEventPtr loggingEvent)
{
   ATLASSERT(loggingEvent.get() != nullptr);

   outputText = m_pattern;

   int currentPos = 0, maxPos = outputText.GetLength();
   while (currentPos < maxPos)
   {
      int charPos = currentPos = outputText.Find(_T('%'), currentPos);
      if (charPos == -1)
         break; // no more format specifiers

      // read format specifier
      charPos++;
      if (charPos >= maxPos)
         break; // finished in the middle of parsing

      TCHAR ch = outputText[charPos];

      bool bLeftJustify = false; // right justify is default

      // read possible left justify char
      if (ch == _T('-'))
      {
         bLeftJustify = true;
         charPos++;
         if (charPos >= maxPos)
            break; // finished

         ch = outputText[charPos];
      }

      /// read min width when available
      int minWidth = -1;
      while (ch >= _T('0') && ch <= _T('9'))
      {
         if (minWidth < 0)
            minWidth = 0;
         else
            minWidth *= 10;
         minWidth += ch - _T('0');

         charPos++;
         if (charPos >= maxPos)
            break; // finished

         ch = outputText[charPos];
      }

      if (charPos >= maxPos)
         break; // finished

      // read point when available
      int maxPosWidth = -1;
      if (ch == _T('.'))
      {
         charPos++;
         if (charPos >= maxPos)
            break; // finished

         ch = outputText[charPos];

         // now read max width
         while (ch >= _T('0') && ch <= _T('9'))
         {
            if (maxPosWidth < 0)
               maxPosWidth = 0;
            else
               maxPosWidth *= 10;
            maxPosWidth += ch - _T('0');

            charPos++;
            if (charPos >= maxPos)
               break; // finished

            ch = outputText[charPos];
         }

         if (charPos >= maxPos)
            break; // finished
      }

      // when we get here, ch contains the specification character
      charPos++; // set pos to beyond char

      // get string to replace with
      CString replaceText;

      switch (ch)
      {
      case _T('c'): // outputs logger name
         replaceText = loggingEvent->LoggerName();
         break;
      case _T('d'): // date in ISO 8601 format
         replaceText = loggingEvent->Timestamp().FormatISO8601(DateTime::formatYMD_HMSF_Z, false);
         break;
      case _T('F'): // source filename where log message occured
         replaceText = loggingEvent->SourceFilename();
         break;
      case _T('l'): // location
         replaceText = loggingEvent->SourceFilename();
         break;
      case _T('L'): // source file line where log message occured
         replaceText.Format(_T("%u"), loggingEvent->SourceLine());
         break;
      case _T('m'): // log message
         replaceText = loggingEvent->Message();
         break;
      case _T('n'): // platform-specific newline character
         replaceText = _T("\n");
         break;
      case _T('p'): // log level (priority)
         switch (loggingEvent->Level())
         {
         case debug: replaceText = _T("DEBUG"); break;
         case info:  replaceText = _T("INFO"); break;
         case warn:  replaceText = _T("WARN"); break;
         case error: replaceText = _T("ERROR"); break;
         case fatal: replaceText = _T("FATAL"); break;
         case none:  replaceText = _T("NONE"); break;
         default:
            ATLASSERT(false);
            break;
         }
         break;
      case _T('r'): // relative time elapsed; not supported
         replaceText = _T("");
         break;
      case _T('t'): // thread id
         replaceText.Format(_T("%u"), loggingEvent->ThreadId());
         break;
      case _T('%'): // percent sign
         replaceText = _T("%");
         break;
      default:
         ATLASSERT(false); // unknown format character
         break;
      }

      // now that we have the string to be replaced, apply min width, max width and left justification
      if (minWidth >= 0)
      {
         // check if we have to pad the string
         int replaceTextLength = replaceText.GetLength();
         if (replaceTextLength < minWidth)
         {
            // yes, pad
            if (bLeftJustify)
               replaceText += CString(_T(' '), minWidth - replaceTextLength); // on the right
            else
               replaceText = CString(_T(' '), minWidth - replaceTextLength) + replaceText; // on the left
         }
      }

      // now check if we have to truncate the string
      if (maxPosWidth >= 0)
      {
         int replaceTextLength = replaceText.GetLength();

         // cut off from the beginning, just as log4cxx does, and different to the printf behavior
         if (replaceTextLength > maxPosWidth)
            replaceText = replaceText.Right(maxPosWidth);
      }

      // delete old string
      outputText.Delete(currentPos, charPos - currentPos);
      // insert new one
      outputText.Insert(currentPos, replaceText);

      currentPos += replaceText.GetLength();
      maxPos = outputText.GetLength();
   }
}
