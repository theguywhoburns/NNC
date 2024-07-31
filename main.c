#include "lexer.h"

int main() {
	token_stream* ts = token_stream_create("main.nnc");
	token token = ts->next(ts);
	while (token.type != TOKEN_EOF) {
		token = ts->next(ts);
	}
	ts->destroy(ts);
	return 0;	
}