#pragma once

#include <Utils.hpp>

class Args final {
public:
	Args() = delete;
	~Args() = delete;

	static void Parse(int argc, char** argv);
	template<typename T>
	static T 		Get(const std::string& name);
	static bool Has(const std::string& name);
	static void Iterate(const std::function<void(const std::string& name, const std::string& value)>& callback);
private:
	static std::unordered_map<std::string, std::string> args;
};