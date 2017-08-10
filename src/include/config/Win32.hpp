//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file Win32.hpp Win32 includes
//
#pragma once

/// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define VC_EXTRALEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOSOUND
#define NOCOMM
#define NOKANJI
#define NORPC
#define NOPROXYSTUB
#define NOTAPE
#define NOCRYPT
#define NOIMAGE

// Win32 includes
#pragma warning(push)
#pragma warning(disable: 4091) // 'typedef ': ignored on left of 'tagGPFIDL_FLAGS' when no variable is declared
#include <shlobj.h>
#include <shellapi.h>
#undef ExtractIcon
#pragma warning(pop)
