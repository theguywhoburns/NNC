#include "lexer.hpp"

int main() {
	TokenStream ts("main.nnc");
	while (ts.next().type != Token::Type::TOKEN_EOF);
	return 0;
}