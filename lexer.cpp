#include "lexer.hpp"

TokenStream::TokenStream(const std::string& path) {
#if _DEBUG
  std::string new_filename = path + ".lexer_out";
  debug_lexer_file = std::ofstream(new_filename);
#endif
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file at path: " + path);
  }
  std::stringstream ss;
  ss << file.rdbuf();
  file_source = ss.str();
  this->last = lex_new();
}

TokenStream::~TokenStream() {
#if _DEBUG
  debug_lexer_file.close();
#endif
}

Token TokenStream::next() {
  Token ret = this->last;
  if (ret.type == Token::Type::TOKEN_EOF) { return ret; } // EOF
  this->last = lex_new();
#if _DEBUG
  std::string ret_str = "[" + token_type_to_string(ret.type) + ":" + std::to_string(ret.line) + ":" + std::to_string(ret.column) + "]: " + std::string(ret.value) + "\n";
  debug_lexer_file.write(ret_str.c_str(), ret_str.size());
  debug_lexer_file.flush();
#endif
  return ret;
}

Token TokenStream::peek() {
  return this->last;
}

std::string TokenStream::token_type_to_string(Token::Type type) {
  switch (type) {
  case Token::Type::TOKEN_EOF: return "EOF";
  case Token::Type::TOKEN_OPAREN: return "OPEN PAREN";
  case Token::Type::TOKEN_CPAREN: return "CLOSE PAREN";
  case Token::Type::TOKEN_OSQUARE: return "OPEN SQUARE";
  case Token::Type::TOKEN_CSQUARE: return "CLOSE SQUARE";
  case Token::Type::TOKEN_OCURLY: return "OPEN CURLY";
  case Token::Type::TOKEN_CCURLY: return "CLOSE CURLY";
  case Token::Type::TOKEN_OANGLE: return "OPEN ANGLE";
  case Token::Type::TOKEN_CANGLE: return "CLOSE ANGLE";
  case Token::Type::TOKEN_IDENTIFIER: return "IDENTIFIER";
  case Token::Type::TOKEN_NUMBER: return "NUMBER";
  case Token::Type::TOKEN_STRING: return "STRING";
  case Token::Type::TOKEN_SPECIAL: return "SPECIAL";
  case Token::Type::TOKEN_PREPROCESSOR: return "PREPROCESSOR";
  default: return "UNKNOWN";
  }
}

Token TokenStream::lex_new() {
  while (file_index < file_source.size() && (file_source[file_index] == ' ' || file_source[file_index] == '\t' || file_source[file_index] == '\n' || file_source[file_index] == '\r' || file_source[file_index] == '/')) {
    switch (file_source[file_index]) {
    case ' ':
    case '\t':
      column++;
      break;
    case '\n':
      line++;
      column = 1;
      break;
    case '\r':
      column = 1;
      break;
    case '/':
      if (file_index < file_source.size() - 1) {
        if (file_source[file_index + 1] == '/') {
          while (file_index < file_source.size() && file_source[file_index] != '\n') {
            file_index++;
          }
          line++;
          column = 1;
        } else if (file_source[file_index + 1] == '*') {
          while (file_index < file_source.size() - 2 && (file_source[file_index] != '*' || file_source[file_index + 1] != '/')) {
            if (file_source[file_index] == '\r') {
              column = 1;
          	} else if (file_source[file_index] == '\n') {
              line++;
            }
            column++;
            file_index++;
          }
          file_index += 2;
          column += 2;
        }
      }
      break;
    default:
      break;
    }
    file_index++;
  }
  if (file_index >= file_source.size() || file_source[file_index] == 0) {
    return Token(Token::Type::TOKEN_EOF, "", line, column);
  }
  switch (file_source[file_index]) {
  case '(': return Token(Token::Type::TOKEN_OPAREN, std::string_view(&file_source[file_index++], 1), line, column++); 
  case ')': return Token(Token::Type::TOKEN_CPAREN, std::string_view(&file_source[file_index++], 1), line, column++);
  case '{': return Token(Token::Type::TOKEN_OCURLY, std::string_view(&file_source[file_index++], 1), line, column++);
  case '}': return Token(Token::Type::TOKEN_CCURLY, std::string_view(&file_source[file_index++], 1), line, column++);
  case '[': return Token(Token::Type::TOKEN_OSQUARE,std::string_view(&file_source[file_index++], 1), line, column++);
  case ']': return Token(Token::Type::TOKEN_CSQUARE,std::string_view(&file_source[file_index++], 1), line, column++);
  case '<': return Token(Token::Type::TOKEN_OANGLE, std::string_view(&file_source[file_index++], 1), line, column++);
  case '>': return Token(Token::Type::TOKEN_CANGLE, std::string_view(&file_source[file_index++], 1), line, column++);
  case '=':
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '&':
  case '|':
  case '^':
  case '~':
  case '!':
  case '?':
  case ':':
  case ',':
  case '.':
  case ';':
    return Token(Token::Type::TOKEN_SPECIAL, std::string_view(&file_source[file_index++], 1), line, column++);
  case '"':
  case '\'':
    return lex_string();
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return lex_number();
  case '#':
    return lex_preprocessor();
  default:
    return lex_identifier();
  }
}

Token TokenStream::lex_number() {
  size_t start = file_index;
  size_t start_column = column;
  while (file_index < file_source.size() && IsNumber(file_source[file_index])) {
    file_index++;
    column++;
  }
  return Token(Token::Type::TOKEN_NUMBER, std::string_view(&file_source[start], file_index - start), line, start_column);
}

Token TokenStream::lex_string() {
  size_t start = file_index;
  size_t start_column = column;
  char string_type = file_source[file_index++];
  column++;
  while (file_index < file_source.size()) {
    char current_char = file_source[file_index++];
    column++;
    if (current_char == '\n') {
      throw std::runtime_error("Lexer error, line " + std::to_string(line) + ", column " + std::to_string(column) + ": Unterminated string");
    }
    if (current_char == string_type) {
      if (string_type == '\'' && file_index - start > 3) {
        throw std::runtime_error("Lexer error, line " + std::to_string(line) + ", column " + std::to_string(column) + ": single quoted string cannot be longer than 1 character");
      }
      break;
    }
    if (current_char == '\\' && file_index < file_source.size()) {
      file_index++;
      column++;
    }
  }
  return Token(Token::Type::TOKEN_STRING, std::string_view(&file_source[start], file_index - start), line, start_column);
}

Token TokenStream::lex_identifier() {
  size_t start_index = file_index;
  size_t start_column = column;
  while (file_index < file_source.size() && (this->IsAlphaNumeric(file_source[file_index]) || file_source[file_index] == '_')) {
    file_index++;
    column++;
  }
  return Token(Token::Type::TOKEN_IDENTIFIER, std::string_view(&file_source[start_index], file_index - start_index), line, start_column);
}

Token TokenStream::lex_preprocessor() {
  size_t start_index = file_index;
  size_t start_column = column;
  while (file_index < file_source.size() && file_source[file_index] != '\n') {
    file_index++;
    column++;
  }
  return Token(Token::Type::TOKEN_PREPROCESSOR, std::string_view(&file_source[start_index], file_index - start_index - 1), line, start_column);
}

bool TokenStream::IsSpecialChar(char c) {
  // TODO: Implement IsSpecialChar
  static const std::string special_chars = "!\"$%&'*+,-./:;=?@\\^|~";
  return special_chars.find(c) != std::string::npos;
}
bool TokenStream::IsSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'; }
bool TokenStream::IsNumber(char c) { return c >= '0' && c <= '9'; }
bool TokenStream::IsLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool TokenStream::IsAlphaNumeric(char c) { return IsLetter(c) || IsNumber(c); }
bool TokenStream::IsNewLine(char c) { return c == '\n'; }