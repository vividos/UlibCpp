//
// ulib - a collection of useful classes
// Copyright (C) 2000-2020 Michael Fink
//
/// \file DynamicLibrary.hpp dynamic library loading
//
#pragma once

/// dynamic library loading
class DynamicLibrary
{
public:
   /// ctor; loads module
   explicit DynamicLibrary(LPCTSTR moduleFilename)
      :m_module(LoadLibrary(moduleFilename), &FreeLibrary)
   {
   }

   /// checks if library is loaded
   bool IsLoaded() const { return m_module != nullptr; }

   /// checks if function with given name is available
   bool IsFunctionAvail(LPCSTR functionName) const
   {
      return IsLoaded() &&
         GetProcAddress(GetModule(), functionName) != nullptr;
   }

   /// returns function with given function name and given function signature
   template <typename Signature>
   Signature GetFunction(LPCSTR functionName) const
   {
      return reinterpret_cast<Signature>(GetProcAddress(GetModule(), functionName));
   }

   /// returns module handle of library
   HMODULE GetModule() const { return reinterpret_cast<HMODULE>(m_module.get()); }

private:
   /// dynamic library module handle
   std::shared_ptr<void> m_module;
};
