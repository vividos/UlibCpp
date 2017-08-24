//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014,2017 Michael Fink
//
/// \file TextStreamAppender.hpp text stream appender class
//
#pragma once

// includes
#include <ulib/log/Appender.hpp>
#include <ulib/stream/ITextStream.hpp>

namespace Log
{
   /// \brief appender that uses a text stream for output
   /// \details outputs formatted text to the given stream
   class TextStreamAppender : public Appender
   {
   public:
      /// ctor
      TextStreamAppender(std::shared_ptr<Stream::ITextStream> textStream)
         :m_textStream(textStream)
      {
      }

      /// dtor
      virtual ~TextStreamAppender() {}

      /// outputs logging event using text stream
      virtual void DoAppend(const LoggingEventPtr loggingEvent)
      {
         ATLASSERT(Layout().get() != nullptr);

         CString outputText;
         Layout()->Format(outputText, loggingEvent);

         m_textStream->Write(outputText);
         m_textStream->WriteEndline();
         m_textStream->Flush();
      }

   private:
      /// text stream
      std::shared_ptr<Stream::ITextStream> m_textStream;
   };


} // namespace Log
