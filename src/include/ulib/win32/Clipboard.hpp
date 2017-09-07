//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2017 Michael Fink
//
/// \file Clipboard.hpp clipboard class
//
#pragma once

#include <vector>

namespace Win32
{
   /// \brief clipboard access
   /// \note when creating this object, the process has exclusive access to the
   /// clipboard, so only create when needed
   class Clipboard
   {
   public:
      /// ctor; opens clipboard
      Clipboard(HWND hwnd = nullptr);

      /// dtor
      ~Clipboard();

      /// empties the clipboard
      void Clear() const;

      /// returns number of formats currently on the clipboard
      int CountFormats() const;

      /// checks if a given format is available
      static bool IsFormatAvail(UINT format);

      /// returns text format (CF_TEXT or CF_UNICODETEXT)
      CString GetText();

      /// returns binary data for given clipboard format
      void GetData(UINT format, std::vector<BYTE>& data);

      /// sets clipboard text as format CF_TEXT or CF_UNICODETEXT (depending on build options)
      void SetText(const CString& text);

      /// sets HTML text
      void SetHtml(const CString& htmlFragment, const CString& sourceURL);

      /// sets clipboard data; generic function
      void SetData(UINT format, const BYTE* dataPtr, UINT size);

      /// registers a given format name
      UINT RegisterFormat(LPCTSTR formatName) const;

      /// returns format name for given format id
      CString GetFormatName(UINT format) const;

      /// enumerates all clipboard formats currently available on the clipboard
      void EnumFormats(std::vector<UINT>& formatList) const;

   private:
      Clipboard(const Clipboard&) = delete;              ///< removed copy ctor
      Clipboard& operator=(const Clipboard&) = delete;   ///< removed assign op
   };

} // namespace Win32
