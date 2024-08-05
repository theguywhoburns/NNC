#include <lexer.hpp>
#include <Args.hpp>

int main(int argc, char** argv) {
	TokenStream ts("main.nnc");
	while (ts.next().type != Token::Type::TOKEN_EOF);
	return 0;
}