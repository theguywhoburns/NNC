#include "lexer.h"

void token_stream_destroy(token_stream* ts);
token token_stream_next(token_stream* ts);
token token_stream_peek(token_stream* ts);

token lex_token(token_stream* ts);
token lex_string(token_stream* ts);
token lex_number(token_stream* ts);
token lex_identifier(token_stream* ts);
token lex_preprocessor(token_stream* ts);

#define WRITE_ERROR(ts, error_msg, ...) printf(error_msg, __VA_ARGS__); assert(false);

bool IsSpecialChar(char c) {
	switch (c) {
	case '~':
	case '`':
	case '!':
	case '@':
	case '#':
	case '$':
	case ';':
	case '%':
	case '^':
	case ':':
	case '&':
	case '?':
	case '*':
	case '-':
	case '=':
	case '+':
	case '\\':
	return true;
		break;
	default:
	return false;
	}
}

bool IsSpace(char c) {
	switch (c) {
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	return true;
		break;
	default:
	return false;
	}
}

bool IsNumber(char c) {
	if(c >= '0' && c <= '9') return true;
	return false;
}

bool IsLetter(char c) {
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return true;
	return false;
}

bool IsUpper(char c) {
	if(c >= 'A' && c <= 'Z') return true;
	return false;
}

bool IsAlphaNumeric(char c) {
	if(IsLetter(c) || IsNumber(c)) return true;
	return false;
}

bool IsNewLine(char c) {
	if(c == '\n' || c == '\r') return true;
	return false;
}

#if _DEBUG
	// Will write the token type to the buffer
	void token_type_to_string(token token, char buffer[32]) {
		if(IS_BRACKET(token)) {
			bool open = IS_OPEN(token);
			switch(token.type & 0b01100000) {
				case TOKEN_PAREN:  strcpy(buffer, open ? "PARENTESIS OPEN"	: "PARENTESIS CLOSED");	break;
				case TOKEN_SQUARE: strcpy(buffer, open ? "SQUARE OPEN"			: "SQUARE CLOSED"); 		break;
				case TOKEN_CURLY:  strcpy(buffer, open ? "CURLY OPEN" 			: "CURLY CLOSED"); 			break;
				case TOKEN_ANGLE:  strcpy(buffer, open ? "ANGLE OPEN"				: "ANGLE CLOSED"); 			break;
				default: assert(false && "Unknown token type... or the universe is going to explode\n");
			}
			return;
			int i = 10;
			switch(i) {
				case 10: 
					printf("10 and ");
				case 11: 
					printf("11\n"); // will be executed both in 10 and 11
					break;
				default: // will not be called
					printf("default\n");
				break;
			}
		} else if(IS_IDENTIFIER(token)) {
			strcpy(buffer, "IDENTIFIER");
		} else if(IS_NUMBER(token)) {
			strcpy(buffer, "NUMBER");
		} else if(IS_STRING(token)) {
			strcpy(buffer, "STRING");
		} else if(IS_SPECIAL(token)) {	
			strcpy(buffer, "SPECIAL");
		} else if(token.type == 0b11111111) {
			strcpy(buffer, "preprocessor... ignore it");
		} else if(token.type == TOKEN_EOF) {
			strcpy(buffer, "EOF");
		} else {
			assert(false && "Unknown token type... or the universe is going to explode lol\n");	
		}
	}
#endif

token_stream *token_stream_create(const char* path) {
	token_stream* ret = malloc(sizeof(token_stream));
	if(ret == NULL) {
		printf("[token_stream_create]: Failed to allocate memory\n"); 
		return NULL;
	}
	*ret = (token_stream){0};
	FILE* file = fopen(path, "rb");
	if(file == NULL) {
		printf("[token_stream_create]: Failed to open file at path: %s\n", path);
		free(ret); 
		return NULL;
	}
	#if _DEBUG
		char* new_filename = malloc(strlen(path) + 11);
		sprintf(new_filename, "%s.lexer_out", path);
		ret->debug_lexer_file = fopen(new_filename, "w");
		if(ret->debug_lexer_file == NULL) {
			printf("[token_stream_create]: Failed to open %s\n", new_filename);
			free(ret);
			free(new_filename);
			fclose(file);
			return NULL;
		}
		free(new_filename);
	#endif
	fseek(file, 0, SEEK_END);
	ret->file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	ret->file_source = malloc(ret->file_size + 1); // +1 for \0
	memset(ret->file_source, 0, ret->file_size + 1);
	if(ret->file_source == NULL) {
		printf("[token_stream_create]: Failed to allocate memory\n");
		free(ret);
		fclose(file);
		return NULL;
	}
	fread(ret->file_source, ret->file_size, 1, file);
	fclose(file);

	ret->next = token_stream_next;
	ret->peek = token_stream_peek;
	ret->destroy = token_stream_destroy;
	ret->last = lex_token(ret);
	return ret;
}

void token_stream_destroy(token_stream* ts) {
	free(ts->file_source);
	free(ts);
}

token token_stream_next(token_stream* ts) {
	token ret = ts->last;
	if(ret.type == TOKEN_EOF) { return ret; } // EOF
	ts->last = lex_token(ts);
	#if _DEBUG
		char _buffer[32] = {0};
		token_type_to_string(ret, _buffer);
		char* buffer = malloc(strlen(ret.value) + 256);
		sprintf(buffer, "[%s:%zu:%zu]: %.*s\n", _buffer, ret.line, ret.column, (int)ret.length, ret.value);
		fwrite(buffer, strlen(buffer), 1, ts->debug_lexer_file);
		free(buffer);
	#endif 
	return ret;
}

token token_stream_peek(token_stream* ts) {
	return ts->last;
}

#define NEW_SINGLECHAR_TOKEN(_type) ret.type = _type; ret.line = ts->line; ret.column = ts->column++; ret.value = &ts->file_source[ts->file_index]; ret.length = 1; ts->file_index++;
#define SINGLECHAR_CASE(_char, _type) case _char: NEW_SINGLECHAR_TOKEN(_type); break
token lex_token(token_stream* ts) {
	token ret = {0}; // default, sets return type to TOKEN_EOF cause it's value is 0b00000000
	while (ts->file_index < ts->file_size && (ts->file_source[ts->file_index] == ' ' || ts->file_source[ts->file_index] == '\t' || ts->file_source[ts->file_index] == '\n' || ts->file_source[ts->file_index] == '\r' || ts->file_source[ts->file_index] == '/')) {
		switch (ts->file_source[ts->file_index]) {
		case ' ':
		case '\t':
			ts->column++;
			break;
		case '\n':
			ts->line++;
			break;
		case '\r':
			ts->column = 0;
			break;
		case '/':
		if(ts->file_index < ts->file_size - 1) {
			if( ts->file_source[ts->file_index + 1] == '/') {
				while(ts->file_index < ts->file_size && ts->file_source[ts->file_index] != '\n') {
					if(ts->file_source[ts->file_index] == '\r') {
						ts->column = 0;
					}
					ts->file_index++;
				}
				ts->line++;
			} else if (ts->file_source[ts->file_index + 1] == '*') {
				while(ts->file_index < ts->file_size - 2 && (ts->file_source[ts->file_index] != '*' && ts->file_source[ts->file_index + 1] != '/')) {
					if(ts->file_source[ts->file_index] == '\r') {
						ts->column = 0;
					} else if(ts->file_source[ts->file_index] == '\n') {
						ts->line++;
					}
					ts->column++;
					ts->file_index++;
				}
				ts->file_index += 2;
				ts->column += 2;
			}
		}
		break;
		default:
			// Wouldn't happen unless universe explodes
			printf("[lex_token]: Wadafuc: %c\n", ts->file_source[ts->file_index]);
			break;
		}
		ts->file_index++;
	}
	if(ts->file_index >= ts->file_size || ts->file_source[ts->file_index] == 0) { return ret; }
	switch (ts->file_source[ts->file_index]){
	SINGLECHAR_CASE('(', TOKEN_BRACKET | TOKEN_PAREN	| TOKEN_OPEN	);
	SINGLECHAR_CASE(')', TOKEN_BRACKET | TOKEN_PAREN	| TOKEN_CLOSED);
	SINGLECHAR_CASE('{', TOKEN_BRACKET | TOKEN_CURLY	| TOKEN_OPEN	);
	SINGLECHAR_CASE('}', TOKEN_BRACKET | TOKEN_CURLY	| TOKEN_CLOSED);
	SINGLECHAR_CASE('[', TOKEN_BRACKET | TOKEN_SQUARE	| TOKEN_OPEN	);
	SINGLECHAR_CASE(']', TOKEN_BRACKET | TOKEN_SQUARE	| TOKEN_CLOSED);
	SINGLECHAR_CASE('<', TOKEN_BRACKET | TOKEN_ANGLE	| TOKEN_OPEN  );
	SINGLECHAR_CASE('>', TOKEN_BRACKET | TOKEN_ANGLE	| TOKEN_CLOSED);
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
		ret.type = TOKEN_SPECIAL;
		ret.line = ts->line;
		ret.column = ts->column;
		ret.value = &ts->file_source[ts->file_index];
		ret.length = 1;
		ts->file_index++;
		ts->column++;
	break;
	case '"':
	case '\'':
		ret = lex_string(ts);
		break;
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
		ret = lex_number(ts);
		break;
	case '#':
		ret = lex_preprocessor(ts);
		break;
	default:
		ret = lex_identifier(ts);
		break;
	}
	return ret;	
}

token lex_number(token_stream* ts) {
	token ret = {0};
	ret.type = TOKEN_NUMBER;
	ret.value = &ts->file_source[ts->file_index];
	ret.length = 0;
	ret.column = ts->column;
	ret.line = ts->line;
	while (ts->file_index < ts->file_size && IsNumber(ts->file_source[ts->file_index])) {
		ts->file_index++;
		ts->column++;
		ret.length++;
	}
	return ret;
}

token lex_string(token_stream* ts) {
	token ret = {0};
	ret.type = TOKEN_STRING;
	ret.value = &ts->file_source[ts->file_index];
	ret.length = 1;
	ret.column = ts->column;
	ret.line = ts->line;
	char string_type = ts->file_source[ts->file_index]; // ' or "
	ts->file_index++;
	ts->column++;

	while (ts->file_index < ts->file_size) {
		char current_char = ts->file_source[ts->file_index];
		ts->file_index++;
		ts->column++;
		ret.length++;    
		if(current_char == '\n') {
			WRITE_ERROR(ts, "Lexer error, line %zu, column %zu: Unterminated string\n", ts->line, ts->column);
		}
		if (current_char == string_type) {
			if(string_type == '\'' && ret.length > 3) {
				WRITE_ERROR(ts, "Lexer error, line %zu, column %zu: single quoted string cannot be longer than 1 character\n", ts->line, ts->column);
			}
			break;
    }

		if (current_char == '\\') {
			if (ts->file_index < ts->file_size) {
				ts->file_index++;
				ts->column++;
				ret.length++;
			}
			continue;
		} 
	}

	return ret;
}

token lex_identifier(token_stream* ts) {
	token ret = {0};
	ret.type = TOKEN_IDENTIFIER;
	ret.value = &ts->file_source[ts->file_index];
	ret.length = 0;
	ret.column = ts->column;
	ret.line = ts->line;
	while (ts->file_index < ts->file_size && (IsAlphaNumeric(ts->file_source[ts->file_index]) || ts->file_source[ts->file_index] == '_')) {
		ts->file_index++;
		ts->column++;
		ret.length++;
	}
	return ret;
}

token lex_preprocessor(token_stream* ts) {
	token ret = {0};
	if(ts->column != 0) {
		if(ts->last.line == ts->line) {
			WRITE_ERROR(ts, "Lexer error, line %zu, column %zu: preprocessor directive must be at the start of a line\n", ts->line, ts->column);
		}
	}

	ret.type = TOKEN_PREPROCESSOR;
	ret.value = &ts->file_source[ts->file_index];
	ret.length = 1;
	ret.column = ts->column;
	ret.line = ts->line;
	ts->file_index++;
	while (ts->file_index < ts->file_size && ts->file_source[ts->file_index] != '\n') {
		if(IsNewLine(ts->file_source[ts->file_index])) {
			if(ts->file_source[ts->file_index - 1] == '\\') {
				ts->file_index++;
				ret.length++;
			} else {
				break;
			}
		}
		ts->file_index++;
		ret.length++;
	}

	return ret;
}