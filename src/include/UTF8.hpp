//
// ulib - a collection of useful classes
// Copyright (C) 2000-2017 Michael Fink
//
/// \file UTF8.hpp UTF-8 conversion
//
#pragma once

#include <vector>

/// converts string to UTF-8 encoding
void StringToUTF8(const CString& text, std::vector<char>& utf8Buffer);

/// converts from UTF-8 encoded text to CString
CString UTF8ToString(const char* utf8Text);
