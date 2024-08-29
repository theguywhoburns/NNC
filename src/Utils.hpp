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
#include <typeindex>
// This one is for reduced memory usage
#include <string_view>

// basically useless but i'll keep it just in case
#include <cassert>
#include <cstdint>

#include <codecvt>

namespace Utils { 
  extern std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	// Custom character functions, fuck off if you don't like them
  inline bool IsSpace (char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'; }
  inline bool IsNumber(char c) { return c >= '0' && c <= '9'; }
  inline bool IsLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
  inline bool IsAlphaNumeric(char c){ return IsLetter(c) || IsNumber(c); }
  inline bool IsNewLine(char c)     { return c == '\n'; }
  inline bool IsSpecialChar(char c) { return std::strchr("!\"$%&'*+,-./:;=?@\\^|~", c) != nullptr; }
  #define MACRO_STR_(x) #x
  #define MACRO_WSTR_(x) L#x
  #define MACRO_UNWRAP(x) x
  #define MACRO_STR(x) MACRO_UNWRAP(MACRO_STR_(x))
  #define MACRO_WSTR(x) MACRO_UNWRAP(MACRO_WSTR_(x))
  #define NNC_VERSION_STR MACRO_STR(NNC_VERSION_MAJOR)"." MACRO_STR(NNC_VERSION_MINOR)"." MACRO_STR(NNC_VERSION_PATCH)
  #define NNC_VERSION_WSTR MACRO_WSTR(NNC_VERSION_MAJOR)L"." MACRO_WSTR(NNC_VERSION_MINOR)L"." MACRO_WSTR(NNC_VERSION_PATCH)
  namespace Ascii {
    ///@author https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
    
    //Regular text
    const inline std::wstring BLACK   =L"\e[0;30m";
    const inline std::wstring RED     =L"\e[0;31m";
    const inline std::wstring GREEN   =L"\e[0;32m";
    const inline std::wstring YELLOW  =L"\e[0;33m";
    const inline std::wstring BLUE    =L"\e[0;34m";
    const inline std::wstring MAGENTA =L"\e[0;35m";
    const inline std::wstring CYAN    =L"\e[0;36m";
    const inline std::wstring WHITE   =L"\e[0;37m";
  
    //Regular bold text
    const inline std::wstring BOLD_BLACK   =L"\e[1;30m";
    const inline std::wstring BOLD_RED     =L"\e[1;31m";
    const inline std::wstring BOLD_GREEN   =L"\e[1;32m";
    const inline std::wstring BOLD_YELLOW  =L"\e[1;33m";
    const inline std::wstring BOLD_BLUE    =L"\e[1;34m";
    const inline std::wstring BOLD_MAGENTA =L"\e[1;35m";
    const inline std::wstring BOLD_CYAN    =L"\e[1;36m";
    const inline std::wstring BOLD_WHITE   =L"\e[1;37m";
  
    //Regular underline text
    const inline std::wstring UNDERLINE_BLACK   =L"\e[4;30m";
    const inline std::wstring UNDERLINE_RED     =L"\e[4;31m";
    const inline std::wstring UNDERLINE_GREEN   =L"\e[4;32m";
    const inline std::wstring UNDERLINE_YELLOW  =L"\e[4;33m";
    const inline std::wstring UNDERLINE_BLUE    =L"\e[4;34m";
    const inline std::wstring UNDERLINE_MAGENTA =L"\e[4;35m";
    const inline std::wstring UNDERLINE_CYAN    =L"\e[4;36m";
    const inline std::wstring UNDERLINE_WHITE   =L"\e[4;37m";
  
    //Regular background
    const inline std::wstring BLACK_BACK   =L"\e[40m";
    const inline std::wstring RED_BACK     =L"\e[41m";
    const inline std::wstring GREEN_BACK   =L"\e[42m";
    const inline std::wstring YELLOW_BACK  =L"\e[43m";
    const inline std::wstring BLUE_BACK    =L"\e[44m";
    const inline std::wstring MAGENTA_BACK =L"\e[45m";
    const inline std::wstring CYAN_BACK    =L"\e[46m";
    const inline std::wstring WHITE_BACK   =L"\e[47m";
  
    //High intensty background 
    const inline std::wstring HIGH_INTENSITY_BLACK_BACK   =L"\e[0;100m";
    const inline std::wstring HIGH_INTENSITY_RED_BACK     =L"\e[0;101m";
    const inline std::wstring HIGH_INTENSITY_GREEN_BACK   =L"\e[0;102m";
    const inline std::wstring HIGH_INTENSITY_YELLOW_BACK  =L"\e[0;103m";
    const inline std::wstring HIGH_INTENSITY_BLUE_BACK    =L"\e[0;104m";
    const inline std::wstring HIGH_INTENSITY_MAGENTA_BACK =L"\e[0;105m";
    const inline std::wstring HIGH_INTENSITY_CYAN_BACK    =L"\e[0;106m";
    const inline std::wstring HIGH_INTENSITY_WHITE_BACK   =L"\e[0;107m";
  
    //High intensty text
    const inline std::wstring HIGH_INTENSITY_BLACK   =L"\e[0;90m";
    const inline std::wstring HIGH_INTENSITY_RED     =L"\e[0;91m";
    const inline std::wstring HIGH_INTENSITY_GREEN   =L"\e[0;92m";
    const inline std::wstring HIGH_INTENSITY_YELLOW  =L"\e[0;93m";
    const inline std::wstring HIGH_INTENSITY_BLUE    =L"\e[0;94m";
    const inline std::wstring HIGH_INTENSITY_MAGENTA =L"\e[0;95m";
    const inline std::wstring HIGH_INTENSITY_CYAN    =L"\e[0;96m";
    const inline std::wstring HIGH_INTENSITY_WHITE   =L"\e[0;97m";
  
    //Bold high intensity text
    const inline std::wstring BOLD_HIGH_INTENSITY_BLACK   =L"\e[1;90m";
    const inline std::wstring BOLD_HIGH_INTENSITY_RED     =L"\e[1;91m";
    const inline std::wstring BOLD_HIGH_INTENSITY_GREEN   =L"\e[1;92m";
    const inline std::wstring BOLD_HIGH_INTENSITY_YELLOW  =L"\e[1;93m";
    const inline std::wstring BOLD_HIGH_INTENSITY_BLUE    =L"\e[1;94m";
    const inline std::wstring BOLD_HIGH_INTENSITY_MAGENTA =L"\e[1;95m";
    const inline std::wstring BOLD_HIGH_INTENSITY_CYAN    =L"\e[1;96m";
    const inline std::wstring BOLD_HIGH_INTENSITY_WHITE   =L"\e[1;97m";
  
    //Reset
    const inline std::wstring RESET =L"\e[0m";
  };
};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  #define OS_WINDOWS 1
#elif defined(unix) || defined(__unix__) || defined(__unix)
  #define OS_UNIX 1
#endif