#pragma once
#include <Utils.hpp>

class Args final{
public:
	static void Parse(int argc, char** argv);
	static bool version;
	static bool verbose;
	static bool help;
	static bool lexer_only;
	static std::wstring output;
	static std::vector<std::wstring> files;
	Args() = delete;
};