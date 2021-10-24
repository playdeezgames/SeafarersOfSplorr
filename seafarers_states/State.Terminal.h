#pragma once
#include <functional>
#include <map>
#include <string>
namespace state
{
	struct Terminal
	{
		static void ClearInput();
		static const std::string& GetInput();
		static const void AppendInput(const std::string&);
		static const bool Backspace();
		static std::function<bool(const std::string&)> DoIntegerInput(const std::map<std::string, std::function<void()>>& table, const std::string& errorMessage, std::function<void()>);
		static void Start();
	};
}