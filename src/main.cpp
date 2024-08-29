/*
#include <Args.hpp>
#include <lexer.hpp>

int main(int argc, char** argv) {
	try {
		Args::Parse(argc, argv);
		if(Args::help) {
			std::cout <<L"Help" << std::endl;
			return 0;
		} else if(Args::version) {
			std::cout << 
			Utils::Ascii::BOLD_HIGH_INTENSITY_WHITE +L"nnc:L" + 
			Utils::Ascii::BOLD_HIGH_INTENSITY_BLUE  + NNC_VERSION_STR + 
			Utils::Ascii::RESET 
			<< std::endl;
			return 0;
		} 
		if(Args::files.empty()) {
			std::cout << 
			Utils::Ascii::BOLD_HIGH_INTENSITY_WHITE +L"nnc:L"		+ 
			Utils::Ascii::BOLD_HIGH_INTENSITY_RED 	+L"error:"	+ 
			Utils::Ascii::RESET +L" No files specified" 
			<< std::endl;
			return -1;
		}
		for(auto& file : Args::files) {
			TokenStream ts(file);
			while (ts.next().type != Token::Type::TOKEN_EOF);
		}
	} catch (std::exception* e) {
		std::cout << 
		Utils::Ascii::BOLD_HIGH_INTENSITY_WHITE +L"nnc:L" + 
		Utils::Ascii::BOLD_HIGH_INTENSITY_RED +L"error:L" + 
		Utils::Ascii::RESET + e->what() << std::endl;
	}
	return 0;
}
*/
#include <iostream>
#include <format>
#include <codecvt>
#include <locale>
#include <Utils.hpp>
#include <Args.hpp>
#include <lexer.hpp>
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> Utils::converter;
// TYPE, REGEX
// Later will swap TYPE from string to enum
const std::vector<std::pair<std::wstring, std::wstring>> rules = {
	{L"MULTICOMMENT", L"\\/\\*([^*]|\\*+[^*\\/])*\\*+\\/"},// Multi-line comment
  {L"COMMENT",   L"\\/\\/.*"},      		// Single-line comment
  {L"PREPROCESSOR",L"#(?s:.)[^\n]*"},
  {L"IDENTIFIER",L"[a-zA-Z_][a-zA-Z0-9_]*"},
  {L"NUMBER",    L"[0-9]+"},
  {L"WSTRING",   L"L\"(\\\\.|[^\\\\\"])*\""},
	{L"STRING",    L"\"(\\\\.|[^\\\\\"])*\""},  // Supports escape sequences in strings
  {L"CHAR",      L"\'(\\\\.|[^\\\\'])\'"},    // Supports escape sequences in chars
  {L"WHITESPACE",L"[ \t\n\r]+"},
  {L"OR",        L"\\|\\|"},// Logical OR
  {L"AND",       L"\\&\\&"},// Logical AND
  {L"BIT_OR",    L"\\|"},// Bitwise OR
  {L"BIT_AND",   L"\\&"},// Bitwise AND
	{L"BIT_XOR",   L"\\^"},// Bitwise XOR
	{L"BIT_NOT",   L"\\~"},// Bitwise NOT
	{L"SHL",       L"<<"}, // Shift left
	{L"SHR",       L">>"}, // Shift right
  {L"PAREN",     L"\\(|\\)"},
  {L"SQUARE",    L"\\[|\\]"},
  {L"CURLY",     L"\\{|\\}"},
  {L"ANGLE",     L"\\<|\\>"},
  {L"NOT",       L"\\!"},
  {L"EQ",        L"\\="},
  {L"PLUS",      L"\\+"},
  {L"MINUS",     L"\\-"},
  {L"MULTIPLY",  L"\\*"},
  {L"DIVIDE",    L"\\/"},
  {L"MODULO",    L"\\%"},
  {L"SEMICOLON", L"\\;"},
  {L"COLON",     L"\\:"},
  {L"DOT",       L"\\."},
  {L"COMMA",     L"\\,"},
	{L"QUESTION",  L"\\?"},
	{L"A FUCKING DOLLAR",L"\\$"},// $
  {L"INVALID",   L"."}              		// Catch-all for invalid tokens
};
int main(int argc, char** argv) {
	try {
		std::locale::global(std::locale(""));
		Args::Parse(argc, argv);
		if(Args::help) {
			std::wcout <<L"Help" << std::endl;
			return 0;
		} else if(Args::version) {
			std::wcout << 
			Utils::Ascii::BOLD_HIGH_INTENSITY_WHITE +L"nnc:" + 
			Utils::Ascii::BOLD_HIGH_INTENSITY_BLUE  + NNC_VERSION_WSTR + 
			Utils::Ascii::RESET 
			<< std::endl;
			return 0;
			auto t = LR"(\\/\\*([^*]|\\*+[^*\\/])*\\*+/)";
		} 
		if(Args::files.empty()) {
			std::wcout << 
			Utils::Ascii::BOLD_HIGH_INTENSITY_WHITE +L"nnc:"		+ 
			Utils::Ascii::BOLD_HIGH_INTENSITY_RED 	+L"error:"	+ 
			Utils::Ascii::RESET +L" No files specified" 
			<< std::endl;
			return -1;
		}
		if(Args::lexer_only) {
			for(auto& file : Args::files) {
				TokenStream ts(file, rules);
				Token t = ts.next();
				std::wofstream out(file +L".lex");
				do {
					std::wcout << L"[" << t.type << L":" << t.line << L":" << t.column << L"]: " << t.value  << std::endl;
					out << L"[" << t.type  << L":" << t.line << L":" << t.column << L"]: " << t.value << std::endl;
				} while((t = ts.next()).type !=L"EOF");
				out << L"[" << t.type  << L":" << t.line << L":" << t.column << L"]: " << t.value << std::endl;
				out.flush();	
				out.close();
			}
		} else {

		}
	} catch (std::exception* e) {
		std::wcout << 
		Utils::Ascii::BOLD_HIGH_INTENSITY_WHITE +L"nnc:L" + 
		Utils::Ascii::BOLD_HIGH_INTENSITY_RED +L"error:L" + 
		Utils::Ascii::RESET + Utils::converter.from_bytes(e->what()) << std::endl;
	}
	return 0;
}