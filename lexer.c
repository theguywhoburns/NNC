#include "lexer.h"

void token_stream_destroy(token_stream* ts);
token token_stream_next(token_stream* ts);
token token_stream_peek(token_stream* ts);

token lex_token(token_stream* ts);
token lex_string(token_stream* ts);
token lex_number(token_stream* ts);

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

token_stream *token_stream_create(const char* path) {
	token_stream* ret = malloc(sizeof(token_stream));
	if(ret == NULL) {
		printf("[token_stream_create]: Failed to allocate memory\n"); 
		return NULL;
	}
	*ret = (token_stream){0};
	FILE* file = fopen(path, "r");
	if(file == NULL) {
		printf("[token_stream_create]: Failed to open file at path: %s\n", path);
		free(ret); 
		return NULL;
	}
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
	return ret;
}

token token_stream_peek(token_stream* ts) {
	return ts->last;
}

#define NEW_SINGLECHAR_TOKEN(_type) ret.type = _type; ts->column++; ret.value = &ts->file_source[ts->file_index]; ret.length = 1; ts->file_index++;
#define SINGLECHAR_CASE(_char, _type) case _char: NEW_SINGLECHAR_TOKEN(_type); break
token lex_token(token_stream* ts) {
	token ret = {0}; // default, sets return type to TOKEN_EOF cause it's value is 0b00000000
	while (ts->file_index < ts->file_size && (ts->file_source[ts->file_index] == ' ' || ts->file_source[ts->file_index] == '\t' || ts->file_source[ts->file_index] == '\n' || ts->file_source[ts->file_index] == '\r')) {
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
				while(ts->file_source[ts->file_index] != '\n') {
					ts->file_index++;
				}
			} else if (ts->file_source[ts->file_index + 1] == '*') {
				while(ts->file_index < ts->file_size - 2 && (ts->file_source[ts->file_index] != '*' && ts->file_source[ts->file_index + 1] != '/')) {
					ts->file_index++;
				}
				ts->file_index += 2;
			}
		}
		default:
			printf("[lex_token]: Wadafuc\n");
			break;
		}
		ts->file_index++;
	}
	if(ts->file_index >= ts->file_size || ts->file_source[ts->file_index] == 0) { return ret; }
	switch (ts->file_source[ts->file_index]){
	SINGLECHAR_CASE('(', TOKEN_OPAREN);
	SINGLECHAR_CASE(')', TOKEN_CPAREN);
	SINGLECHAR_CASE('{', TOKEN_OBRACE);
	SINGLECHAR_CASE('}', TOKEN_CBRACE);
	SINGLECHAR_CASE('[', TOKEN_OCURLY);
	SINGLECHAR_CASE(']', TOKEN_CCURLY);
	SINGLECHAR_CASE('<', TOKEN_OANGLE);
	SINGLECHAR_CASE('>', TOKEN_CANGLE);

	SINGLECHAR_CASE('=', TOKEN_SYMBOL);
	SINGLECHAR_CASE('+', TOKEN_SYMBOL);
	SINGLECHAR_CASE('-', TOKEN_SYMBOL);
	SINGLECHAR_CASE('*', TOKEN_SYMBOL);
	SINGLECHAR_CASE('/', TOKEN_SYMBOL);
	SINGLECHAR_CASE('%', TOKEN_SYMBOL);
	SINGLECHAR_CASE('&', TOKEN_SYMBOL);
	SINGLECHAR_CASE('|', TOKEN_SYMBOL);
	SINGLECHAR_CASE('^', TOKEN_SYMBOL);
	SINGLECHAR_CASE('~', TOKEN_SYMBOL);
	SINGLECHAR_CASE('!', TOKEN_SYMBOL);
	SINGLECHAR_CASE('?', TOKEN_SYMBOL);
	SINGLECHAR_CASE(':', TOKEN_SYMBOL);
	SINGLECHAR_CASE(',', TOKEN_SYMBOL);
	SINGLECHAR_CASE('.', TOKEN_SYMBOL);
	SINGLECHAR_CASE(';', TOKEN_SYMBOL);

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
	default:
		break;
	}
	
	
	return ret;	
}

token lex_number(token_stream* ts) {
	token ret = {0};
	ret.type = TOKEN_NUMBER;
	ret.value = &ts->file_source[ts->file_index];
	ret.length = 1;
	while (ts->file_index < ts->file_size && ts->file_source[ts->file_index] >= '0' && ts->file_source[ts->file_index] <= '9') {
		ts->file_index++;
		ret.length++;
	}
	
	return ret;
}

token lex_string(token_stream* ts) {
	token ret = {0};
	ret.type = TOKEN_STRING;
	ret.value = &ts->file_source[ts->file_index];
	ret.length = 1;

	char string_type = ts->file_source[ts->file_index]; // ' or "
	ts->file_index++;
	ts->column++;

	while (ts->file_index < ts->file_size) {
		char current_char = ts->file_source[ts->file_index];
		if (current_char == string_type) {
			ret.length++;
			ts->file_index++;
			ts->column++;
			break;
    }
		
		if (current_char == '\\') {
			ts->file_index++;
			ts->column++;
			ret.length++;
			if (ts->file_index < ts->file_size) {
				ts->file_index++;
				ts->column++;
				ret.length++;
			}
			continue;
		} 
		ts->file_index++;
		ts->column++;
		ret.length++;    
	}
	return ret;
}