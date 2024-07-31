#pragma once

#include "stdincludes.h"

#define TOKEN_EOF 				0b00000000
// any bracket like ()[]{}<>
#define TOKEN_BRACKET	0b00000001
// ([{<
#define TOKEN_OPEN		0b10000000
// >}])
#define TOKEN_CLOSED	0b00000000
// ... yeah, this is needed ( or )
#define TOKEN_PAREN		0b00000000
// [ or ]
#define TOKEN_SQUARE	0b01000000
// { or }
#define TOKEN_CURLY		0b00100000
// < or >
#define TOKEN_ANGLE		0b01100000

#define TOKEN_SPECIAL			0b00000010
#define TOKEN_IDENTIFIER	0b00000011
#define TOKEN_NUMBER			0b00000100
#define TOKEN_STRING			0b00000101

#define TOKEN_PREPROCESSOR	0b11111111

#define IS_BRACKET_(token_type)			(((token_type) & 0b00000111) == TOKEN_BRACKET)
#define IS_PAREN_(token_type)				(((token_type) & 0b01100000) == TOKEN_PAREN)
#define IS_SQUARE_(token_type)			(((token_type) & 0b01100000) == TOKEN_SQUARE)
#define IS_CURLY_(token_type)				(((token_type) & 0b01100000) == TOKEN_CURLY)
#define IS_ANGLE_(token_type)				(((token_type) & 0b01100000) == TOKEN_ANGLE)

#define IS_OPEN_(token_type)  			(((token_type) & 0b10000000) == TOKEN_OPEN)
#define IS_CLOSED_(token_type)			(((token_type) & 0b10000000) == TOKEN_CLOSED)

#define IS_SPECIAL_(token_type)			(((token_type) & 0b00000111) == TOKEN_SPECIAL)
#define IS_IDENTIFIER_(token_type)	(((token_type) & 0b00000111) == TOKEN_IDENTIFIER)
#define IS_NUMBER_(token_type)			(((token_type) & 0b00000111) == TOKEN_NUMBER)
#define IS_STRING_(token_type)			(((token_type) & 0b00000111) == TOKEN_STRING)



#define IS_BRACKET(_token)		IS_BRACKET_((_token).type)
#define IS_PAREN(_token)  		IS_PAREN_((_token).type)
#define IS_SQUARE(_token)			IS_SQUARE_((_token).type)
#define IS_CURLY(_token)			IS_CURLY_((_token).type)
#define IS_ANGLE(_token)			IS_ANGLE_((_token).type)

#define IS_OPEN(_token)				IS_OPEN_((_token).type)
#define IS_CLOSED(_token)			IS_CLOSED_((_token).type)

#define IS_SPECIAL(_token)		IS_SPECIAL_((_token).type)
#define IS_IDENTIFIER(_token)	IS_IDENTIFIER_((_token).type)
#define IS_NUMBER(_token)			IS_NUMBER_((_token).type)
#define IS_STRING(_token)			IS_STRING_((_token).type)

bool IsSpecialChar(char c);
bool IsSpace(char c);
bool IsNumber(char c);
bool IsLetter(char c);
bool IsUpper(char c);
bool IsAlphaNumeric(char c);

typedef struct token {
	int type;
	char* value;
	size_t line;
	size_t column;
	size_t length;
} token;

typedef struct token_stream {
	/// @brief returns ts->last and lexes new token
	token  (*next)(struct token_stream*);
	/// @brief returns ts->last
	token  (*peek)(struct token_stream*);
	void(*destroy)(struct token_stream*);
	size_t line;
	size_t column;
	char* file_source;
	size_t file_size;
	size_t file_index;
	token last;
	#if _DEBUG
		FILE* debug_lexer_file;
	#endif
} token_stream;

token_stream *token_stream_create(const char* path);

#if _DEBUG
	// Will write the token type to the buffer
	void token_type_to_string(token token, char buffer[32]);
#endif