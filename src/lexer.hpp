#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>

class Token {
public:
  std::wstring value;
  std::wstring type;
  size_t line;
  size_t column;

  Token(std::wstring value, std::wstring type, size_t line, size_t column)
    : value(value), type(type), line(line), column(column) {}
};

class TokenStream {
public:
  TokenStream(const std::wstring& path, const std::vector<std::pair<std::wstring, std::wstring>>& rules);
  ~TokenStream();
  Token next();
  Token peek();

private:
  Token lex_new();
  void skip_whitespace();
  void report_error(const std::wstring& message, size_t line, size_t column);

private:
  std::vector<std::pair<std::wstring, std::wstring>> rules;
  std::wstring file_source;
  size_t file_index = 0;
  size_t line  = 1;
  size_t column = 1;
  Token last = Token(L"",L"EOF", 0, 0);
};
