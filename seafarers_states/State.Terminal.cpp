#include <Common.Utility.Dispatcher.h>
#include <Game.Colors.h>
#include "State.Terminal.h"
#include <Visuals.Terminals.h>
namespace state
{
	static std::string inputBuffer = "";
	static const std::string LAYOUT_NAME = "State.Terminal";
	static const std::string TERMINAL_ID = "terminal";

	void Terminal::ClearInput()
	{
		inputBuffer = "";
	}

	const std::string& Terminal::GetInput()
	{
		return inputBuffer;
	}

	const void Terminal::AppendInput(const std::string& text)
	{
		inputBuffer += text;
	}

	const bool Terminal::Backspace()
	{
		if (inputBuffer.empty())
		{
			return false;
		}
		else
		{
			inputBuffer.pop_back();
			return true;
		}
	}

	void Terminal::Start()
	{
		ClearInput();
	}

	static const std::string KEY_RETURN = "Return";
	static const std::string KEY_1 = "1";
	static const std::string KEY_2 = "2";
	static const std::string KEY_3 = "3";
	static const std::string KEY_4 = "4";
	static const std::string KEY_5 = "5";
	static const std::string KEY_6 = "6";
	static const std::string KEY_7 = "7";
	static const std::string KEY_8 = "8";
	static const std::string KEY_9 = "9";
	static const std::string KEY_0 = "0";
	static const std::string KEY_BACKSPACE = "Backspace";


	std::function<bool(const std::string&)> Terminal::DoIntegerInput(const std::map<std::string, std::function<void()>>& table, const std::string& errorMessage, std::function<void()> refresh)
	{
		return [errorMessage, table, refresh](const std::string& key)
		{
			if (
				key == KEY_1 ||
				key == KEY_2 ||
				key == KEY_3 ||
				key == KEY_4 ||
				key == KEY_5 ||
				key == KEY_6 ||
				key == KEY_7 ||
				key == KEY_8 ||
				key == KEY_9 ||
				key == KEY_0)
			{
				Terminal::AppendInput(key);
				visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, key);
				return true;
			}
			else if (key == KEY_RETURN)
			{
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
				bool result = common::utility::Dispatcher::Dispatch(table, Terminal::GetInput(), true, false);
				if (result)
				{
					Terminal::ClearInput();
				}
				else
				{
					visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
					visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::RED);
					visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, errorMessage);
					visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::GRAY);

					refresh();
				}
				return true;
			}
			else if (key == KEY_BACKSPACE)
			{
				if (Terminal::Backspace())
				{
					visuals::Terminals::Backspace(LAYOUT_NAME, TERMINAL_ID);
				}
				return true;
			}
			return false;

		};
	}
}