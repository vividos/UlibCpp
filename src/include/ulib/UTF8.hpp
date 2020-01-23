//
// ulib - a collection of useful classes
// Copyright (C) 2000-2020 Michael Fink
//
/// \file UTF8.hpp UTF-8 conversion
//
#pragma once

#include <vector>

/// converts string to UTF-8 encoding
/// \returns the resulting buffer always contains a zero character as string terminator
void StringToUTF8(const CString& text, std::vector<char>& utf8Buffer);

/// converts from UTF-8 encoded text to CString; the text must be zero terminated
CString UTF8ToString(const char* utf8Text);
