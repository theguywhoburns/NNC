#include <lexer.hpp>

TokenStream::TokenStream(const std::wstring& path, const std::vector<std::pair<std::wstring, std::wstring>>& rules) {
  std::wifstream file(path, std::ios::in | std::ios::binary);

  //TODO: Runtime error with wstring
  if (!file.is_open()) throw std::runtime_error(std::string("Failed to open file at path:") + std::string(path.begin(), path.end()));
  file_source = std::wstring(std::istreambuf_iterator<wchar_t>(file), std::istreambuf_iterator<wchar_t>());
  this->rules = rules;
  this->last = lex_new();
}

TokenStream::~TokenStream() {}

Token TokenStream::next() {
  Token current = last;
  last = lex_new();
  return current;
}

Token TokenStream::peek() {
  return last;
}

Token TokenStream::lex_new() {
  skip_whitespace();

  if (file_index >= file_source.size()) {
    return Token(L"",L"EOF", line, column);
  }

  for (const auto& [type, pattern] : rules) {
    std::wregex regex(pattern);
    std::wsmatch match;

    std::wstring remaining_source = file_source.substr(file_index);

    if (std::regex_search(remaining_source, match, regex, std::regex_constants::match_continuous)) {
      std::wstring matched_value = match.str(0);
      size_t token_start_line = line;
      size_t token_start_column = column;

    	// Move the file index, line, and column
    	file_index += matched_value.size();
    	for (wchar_t c : matched_value) {
    	  if (c == L'\n') {
    	    line++;
    	    column = 1;
    	  } else {
    	    column++;
    	  }
      }
      // Handle CHAR validation
      if (type ==L"CHAR") {
        if (matched_value.size() != 3) {
          report_error(L"Invalid character literal: too many characters", token_start_line, token_start_column);
          return Token(matched_value,L"INVALID", token_start_line, token_start_column);
        }
      }

      return Token(matched_value, type, token_start_line, token_start_column);
    }
  }

  // If no match is found
  report_error(L"Invalid token", line, column);
  return Token(std::wstring(1, file_source[file_index++]),L"INVALID", line, column);
}

void TokenStream::skip_whitespace() {
  while (file_index < file_source.size() && std::isspace(file_source[file_index])) {
    if (file_source[file_index] == '\n') {
      line++;
      column = 1;
    } else {
      column++;
    }
    file_index++;
  }
}

void TokenStream::report_error(const std::wstring& message, size_t line, size_t column) {
  std::wcerr <<L"Error at line" << line <<L", column" << column <<L":" << message << std::endl;
}