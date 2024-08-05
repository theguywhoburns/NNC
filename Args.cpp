#include <Args.hpp>
/**
 * Args syntax:
 * -# single char
 * --## multiple chars
 * -#= assign value
 * -# ' ' assign value
 * --##= assign value
 * --## ' ' assign value
 * example: -h --help -o output.nnc || -o=output.nnc 
*/
// TODO: implement this and fix the problem with no '-' args
void Args::Parse(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::string_view arg = argv[i];
		if(arg.length() == 0) { throw std::invalid_argument("Empty argument"); }
	}
}

template<typename T>
T Args::Get(const std::string& name) {
	if (!Args::Has(name)) throw std::invalid_argument("Argument name not found");
	return std::get<T>(args[name]);
}

bool Args::Has(const std::string& name) {
	return args.find(name) != args.end();
}

void Args::Iterate(const std::function<void(const std::string& name, const std::string& value)>& callback) {
	for(auto& arg : args) callback(arg.first, arg.second);
}