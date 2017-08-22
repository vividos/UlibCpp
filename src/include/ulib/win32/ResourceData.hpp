//
// ulib - a collection of useful classes
// Copyright (C) 2004,2005,2006,2007,2008,2011,2017 Michael Fink
//
/// \file ResourceData.hpp resource data class
//
#pragma once

// includes
#include <vector>

namespace Win32
{
   /// \brief Resource data helper
   /// This class extracts resource data (that may be stored as RT_RCDATA type)
   /// and extracts it as string or file on disk.
   class ResourceData
   {
   public:
      /// ctor
      ResourceData(LPCTSTR resourceName, LPCTSTR resourceType = _T("\"RT_RCDATA\""), HINSTANCE instanceHandle = nullptr);

      /// returns true if the resource is available
      bool IsAvailable() const;

      /// returns resource data as byte array
      bool AsRawData(std::vector<BYTE>& rawData);

      /// returns resource data as string
      CString AsString(bool storedAsUnicode = false);

      /// saves resource data as file
      bool AsFile(LPCTSTR filename);

   private:
      /// retrieves resource as void pointer and returns size
      LPVOID GetResource(DWORD& size);

      /// closes resource handle
      void CloseResource();

   private:
      /// resource name
      LPCTSTR m_resourceName;

      /// resource type
      LPCTSTR m_resourceType;

      /// locked global memory pointer to resource
      HGLOBAL m_global;

      /// size of resource
      DWORD m_size;

      /// instance handle to search for resource
      HINSTANCE m_instanceHandle;
   };

} // namespace Win32
