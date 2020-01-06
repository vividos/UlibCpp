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
      :m_module(LoadLibrary(moduleFilename))
   {
   }

   /// copy ctor; not available
   DynamicLibrary(const DynamicLibrary& other) = delete;

   // move ctor
   DynamicLibrary(DynamicLibrary&& other) noexcept
      :m_module(std::move(other.m_module))
   {
   }

   /// dtor; frees module again
   ~DynamicLibrary()
   {
      FreeLibrary(m_module);
   }

   /// copy assignment operator; not available
   DynamicLibrary& operator=(const DynamicLibrary& other) = delete;

   /// move assignment operator
   DynamicLibrary& operator=(DynamicLibrary&& other) noexcept
   {
      m_module = std::move(other.m_module);
      return *this;
   }

   /// checks if library is loaded
   bool IsLoaded() const { return m_module != nullptr; }

   /// checks if function with given name is available
   bool IsFunctionAvail(LPCSTR functionName)
   {
      return GetProcAddress(m_module, functionName) != nullptr;
   }

   /// returns function with given function name and given function signature
   template <typename Signature>
   Signature GetFunction(LPCSTR functionName)
   {
      return reinterpret_cast<Signature>(GetProcAddress(m_module, functionName));
   }

private:
   /// dynamic library module handle
   HMODULE m_module;
};
