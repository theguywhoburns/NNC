#include "lexer.hpp"

int main() {
	TokenStream ts("main.nnc");
	Token t = { Token::Type::TOKEN_UNKNOWN, "", 0, 0 };
	while ((t = ts.next()).type != Token::Type::TOKEN_EOF) {
		std::cout << t.value << std::endl;
	}
	return 0;
}