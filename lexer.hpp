#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cstdint>
#include <unordered_map>
#include <cassert>
#include <string>
#include <string_view>

class Token {
public:
  enum class Type {
    TOKEN_EOF = 0,
    TOKEN_OPAREN,
    TOKEN_CPAREN,
    TOKEN_OSQUARE,
    TOKEN_CSQUARE,
    TOKEN_OCURLY,
    TOKEN_CCURLY,
    TOKEN_OANGLE,
    TOKEN_CANGLE,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SPECIAL,
    TOKEN_UNKNOWN,
    TOKEN_PREPROCESSOR
  };
  Token(Type type, std::string_view value, size_t line, size_t column)
    : type(type), value(value), line(line), column(column) {}
  Type type;
  std::string_view value;
  size_t line;
  size_t column;
};

class TokenStream {
public:
  TokenStream(const std::string& path);
  ~TokenStream();
  Token next();
  Token peek();

private:
  std::string file_source;
  size_t file_index = 0;
  size_t line = 1;
  size_t column = 1;
  Token last = Token(Token::Type::TOKEN_EOF, "", 0, 0);
 	Token lex_new();
  Token lex_number();
  Token lex_string();
  Token lex_identifier();
  Token lex_preprocessor();

  bool IsSpecialChar(char c);
  bool IsSpace(char c);
  bool IsNumber(char c);
  bool IsLetter(char c);
  bool IsAlphaNumeric(char c);
  bool IsNewLine(char c);

#if _DEBUG
  std::ofstream debug_lexer_file;
  std::string token_type_to_string(Token::Type type);
#endif
};