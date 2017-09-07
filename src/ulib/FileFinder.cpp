//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file FileFinder.cpp File finder
//

// includes
#include "stdafx.h"
#include <ulib/FileFinder.hpp>
#include <ulib/Path.hpp>

std::vector<CString> FileFinder::FindAllInPath(const CString& path, const CString& fileSpec, bool findFolders, bool recursive)
{
   std::vector<CString> filenamesList;

   FileFinder finder(path, fileSpec);
   if (finder.IsValid())
   {
      do
      {
         if (finder.IsDot())
            continue;

         if (recursive && finder.IsFolder())
         {
            std::vector<CString> subfolderFilenamesList =
               FindAllInPath(finder.Filename(), fileSpec, findFolders, true);

            if (!subfolderFilenamesList.empty())
               filenamesList.insert(filenamesList.end(), subfolderFilenamesList.begin(), subfolderFilenamesList.end());
         }

         if (findFolders && finder.IsFile())
            continue;

         if (!findFolders && finder.IsFolder())
            continue;

         filenamesList.push_back(finder.Filename());

      } while (finder.Next());
   }

   return filenamesList;
}
