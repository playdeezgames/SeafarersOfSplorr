#pragma once
#include <format>
#include <functional>
#include <map>
#include "Menu.h"
#include <string>
namespace state
{
	struct Terminal
	{
		Terminal() = delete;
		static Menu menu;
		static const std::string INVALID_INPUT;
		static void ClearInput();
		static const std::string& GetInput();
		static void AppendInput(const std::string&);
		static void HandleBackspace();
		static const bool Backspace();
		static std::function<bool(const std::string&)> DoMenuInput(
			const std::string&,
			std::function<void()>);
		static std::function<bool(const std::string&)> DoIntegerInput(
			const std::map<std::string, std::function<void()>>&,
			std::function<void(const std::string&)>);
		static std::function<bool(const std::string&)> DoIntegerInput(
			const std::map<std::string, std::function<void()>>&, 
			const std::string&, 
			std::function<void()>);
		static std::function<bool(const std::string&)> DoDoubleInput(std::function<void()>, std::function<void(double)>);
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
		template<typename... Ts>
		static void Write(const std::string& fmt, Ts... args)
		{
			Write(std::format(fmt, args...));
		}
		static void SetForeground(const std::string_view&);
		static void ClearStatusLine();
		static void ShowPrompt();
		static void Reinitialize();
		static void ErrorMessage(const std::string&);
		static void ShowMenu();
	};
}