#pragma once
#include <iostream>
#include <fstream>

// Currently unused
#include <sstream>
#include <chrono>
#include <filesystem>
#include <thread> 
//

#include <string>
#include <vector>

#include <limits>
#include <unordered_map>
#include <functional>
// This one is for reduced memory usage
#include <string_view>

// basically useless but i'll keep it just in case
#include <cassert>
#include <cstdint>

namespace Utils { 
	// Custom character functions, fuck off if you don't like them
  inline bool IsSpace (char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'; }
  inline bool IsNumber(char c) { return c >= '0' && c <= '9'; }
  inline bool IsLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
  inline bool IsAlphaNumeric(char c){ return IsLetter(c) || IsNumber(c); }
  inline bool IsNewLine(char c)     { return c == '\n'; }
  inline bool IsSpecialChar(char c) { return std::strchr("!\"$%&'*+,-./:;=?@\\^|~", c) != nullptr; }
};