#include <Args.hpp>
#include <codecvt>
bool Args::help = false;
bool Args::version = false;
bool Args::verbose = false;
bool Args::lexer_only = false;
std::vector<std::wstring> Args::files;
std::wstring Args::output = 
#if OS_WINDOWS
	L"a.exe"
#elif OS_UNIX
	L"a.out"
#endif
;
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
		std::wstring arg = Utils::converter.from_bytes(argv[i]);
		if(arg.length() == 0) { throw std::invalid_argument("Empty argument"); }
		if(arg ==L"-h" || arg ==L"--help") { help = true; }
		else if(arg ==L"-v" || arg ==L"--version") { version = true; }	
		else if(arg ==L"-V" || arg ==L"--verbose") { verbose = true; }
		else if(arg ==L"-o" || arg ==L"--output") {
			if(++i < argc) output = Utils::converter.from_bytes(argv[i]);
			else throw std::invalid_argument("No output specified");
		} else if(arg ==L"-L" || arg ==L"--lex") { lexer_only = true; } 
		else if(arg.find('-') != std::wstring::npos) {
			throw std::invalid_argument(std::string("Invalid argument: " + Utils::converter.to_bytes(arg)));
		} else {
			files.push_back(std::wstring(arg));
		}
	}
}