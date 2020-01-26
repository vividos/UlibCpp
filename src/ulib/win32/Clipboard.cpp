//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2017,2020 Michael Fink
//
/// \file Clipboard.cpp clipboard class
//
#pragma once

#include "stdafx.h"
#include <ulib/win32/Clipboard.hpp>
#include <ulib/UTF8.hpp>

using Win32::Clipboard;

Clipboard::Clipboard(HWND hwnd)
{
   ATLVERIFY(TRUE == OpenClipboard(hwnd));
}

Clipboard::~Clipboard()
{
   ATLVERIFY(TRUE == CloseClipboard());
}

void Clipboard::Clear() const
{
   ATLVERIFY(TRUE == EmptyClipboard());
}

int Clipboard::CountFormats() const
{
   return CountClipboardFormats();
}

bool Clipboard::IsFormatAvail(UINT format)
{
   return FALSE != IsClipboardFormatAvailable(format);
}

CString Clipboard::GetText()
{
#ifdef _UNICODE
   UINT format = CF_TEXT;
#else
   UINT format = CF_UNICODETEXT;
#endif
   HANDLE hglb = GetClipboardData(format);
   if (hglb != nullptr)
   {
      LPCVOID dataPtr = GlobalLock(hglb);
      if (dataPtr != nullptr)
      {
         CString text(reinterpret_cast<LPCTSTR>(dataPtr));

         GlobalUnlock(hglb);

         return text;
      }
   }

   return CString();
}

void Clipboard::GetData(UINT format, std::vector<BYTE>& data)
{
   ATLASSERT(TRUE == IsClipboardFormatAvailable(format));

   HANDLE hglb = GetClipboardData(format);
   if (hglb != nullptr)
   {
      LPCVOID dataPtr = GlobalLock(hglb);
      if (dataPtr != nullptr)
      {
         SIZE_T size = GlobalSize(hglb);

         data.resize(size);
         memcpy(data.data(), dataPtr, size);

         GlobalUnlock(hglb);
      }
   }
}

void Clipboard::SetText(const CString& text)
{
   USES_CONVERSION;
#if 0 //#ifdef _UNICODE
   CStringW wideText = text;
   SetData(CF_UNICODETEXT,
      reinterpret_cast<const BYTE*>(wideText.GetString()),
      static_cast<UINT>((wideText.GetLength() + 1) * sizeof(WCHAR)));
#else
   CStringA ansiText = text;
   SetData(CF_TEXT,
      reinterpret_cast<const BYTE*>(ansiText.GetString()),
      static_cast<UINT>((ansiText.GetLength() + 1) * sizeof(CHAR)));
#endif
}

void Clipboard::SetHtml(const CString& htmlFragment, const CString& sourceURL)
{
   UNUSED(htmlFragment);
   UNUSED(sourceURL);

   ATLASSERT(false);
}

void Clipboard::SetData(UINT format, const BYTE* dataPtr, UINT size)
{
   Clear();

   HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, size);

   if (hglbCopy != nullptr)
   {
      LPVOID clipboardDataPtr = GlobalLock(hglbCopy);
      if (clipboardDataPtr != nullptr)
         memcpy(clipboardDataPtr, dataPtr, size);

      GlobalUnlock(hglbCopy);

      SetClipboardData(format, hglbCopy);
   }
}

UINT Clipboard::RegisterFormat(LPCTSTR formatName) const
{
   return RegisterClipboardFormat(formatName);
}

CString Clipboard::GetFormatName(UINT format) const
{
   CString name;
   ATLVERIFY(TRUE == GetClipboardFormatName(format, name.GetBufferSetLength(256), 256));
   name.ReleaseBuffer();

   return name;
}

void Clipboard::EnumFormats(std::vector<UINT>& formatList) const
{
   UINT format = 0;
   while (0 != (format = EnumClipboardFormats(format)))
      formatList.push_back(format);
}
