#pragma once
#include <format>
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
		static const std::string LAYOUT_NAME;
		static const std::string TERMINAL_ID;
		static const std::string STATUS_ID;
		static void WriteLine();
		static void WriteLine(const std::string&);
		template<typename... Ts>
		static void WriteLine(const std::string& fmt, Ts... args)
		{
			WriteLine(std::format(fmt, args...));
		}
		static void Write(const std::string&);
		static void SetForeground(const std::string&);
		static void ClearStatusLine();
		static void ShowPrompt();
		static void Reinitialize();
	};
}