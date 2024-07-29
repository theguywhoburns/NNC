#pragma once

#include "stdincludes.h"

#define TOKEN_EOF 				0b00000000

#define TOKEN_IDENTIFIER	0b00000001
#define TOKEN_STRING 0b00000010
#define TOKEN_NUMBER 0b00000100
#define TOKEN_SYMBOL 0b00001000

#define TOKEN_OPAREN 0b00010000
#define TOKEN_CPAREN 0b00100000
#define TOKEN_OBRACE 0b01010000
#define TOKEN_CBRACE 0b01100000
#define TOKEN_OCURLY 0b10010000
#define TOKEN_CCURLY 0b10100000
#define TOKEN_OANGLE 0b11010000
#define TOKEN_CANGLE 0b11100000

#define IS_OPEN (_token_arg)  (((_token_arg.type) & 0b00110000) == 0b00010000)
#define IS_CLOSE(_token_arg)  (((_token_arg.type) & 0b00110000) == 0b00100000)
#define IS_PAREN(_token_arg)  (((_token_arg.type) & 0b11000000) == 0b00000000)
#define IS_BRACE(_token_arg)  (((_token_arg.type) & 0b11000000) == 0b01000000)
#define IS_CURLY(_token_arg)  (((_token_arg.type) & 0b11000000) == 0b10000000)
#define IS_ANGLE(_token_arg)  (((_token_arg.type) & 0b11000000) == 0b11000000)

#define IS_NUMBER(_token_arg) ((_token_arg.type) == 0b00000100)
#define IS_STRING(_token_arg) ((_token_arg.type) == 0b00000010)
#define IS_SYMBOL(_token_arg) ((_token_arg.type) == 0b00001000)

#define IS_EOF(_token_arg) ((_token_arg.type) == 0b00000000)

bool IsSpecialChar(char c);
bool IsSpace(char c);
bool IsNumber(char c);
bool IsLetter(char c);
bool IsUpper(char c);
bool IsAlphaNumeric(char c);

typedef struct token {
	int type;
	char* value;
	size_t length;
} token;

typedef struct token_stream {
	size_t line;
	size_t column;
	char* file_source;
	size_t file_size;
	size_t file_index;
	token last;
	/// @brief returns ts->last and lexes new token
	token  (*next)(struct token_stream*);
	/// @brief returns ts->last
	token  (*peek)(struct token_stream*);
	void(*destroy)(struct token_stream*);
} token_stream;

token_stream *token_stream_create(const char* path);
