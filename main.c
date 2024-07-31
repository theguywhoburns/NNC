#include "lexer.h"

int main() {
	token_stream* ts = token_stream_create("main.nnc");
	token token = ts->next(ts);
	while (token.type != TOKEN_EOF) {
		printf("[%zu:%zu]: %.*s\n", token.line, token.column, (int)token.length, token.value);
		token = ts->next(ts);
	}
	ts->destroy(ts);
	return 0;	
}