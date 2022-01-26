#include "State.Terminal.h"
#include <Common.Data.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Colors.h>
#include <Game.Session.Player.h>
#include <set>
#include <Visuals.Terminals.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Messages.h>
namespace state
{
	Menu Terminal::menu;

	const std::string Terminal::INVALID_INPUT = "Please select a valid option.";

	static std::string inputBuffer = "";
	const std::string Terminal::LAYOUT_NAME = "State.Terminal";
	const std::string Terminal::TERMINAL_ID = "terminal";
	const std::string Terminal::STATUS_ID = "status";

	void Terminal::ShowMenu()
	{
		SetForeground(game::Colors::YELLOW);
		auto items = menu.GetCurrentPage();
		std::for_each(
			items.begin(),
			items.end(),
			[](const std::string& item) 
			{
				WriteLine(item);
			});
	}

	void Terminal::ClearInput()
	{
		inputBuffer = "";
	}

	const std::string& Terminal::GetInput()
	{
		return inputBuffer;
	}

	void Terminal::AppendInput(const std::string& text)
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

	void Terminal::SetForeground(const std::string_view& color)
	{
		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, color);
	}

	void Terminal::ClearStatusLine()
	{
		visuals::Terminals::WriteLine(LAYOUT_NAME, STATUS_ID, "");
	}

	void Terminal::Write(const std::string& text)
	{
		visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, text);
	}

	void Terminal::WriteLine(const std::string& text)
	{
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, text);
	}

	void Terminal::WriteLine()
	{
		WriteLine("");
	}

	static const std::string KEY_RETURN = "Return";
	static const std::string KEYPAD_ENTER = "Keypad Enter";
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
	static const std::string KEYPAD_1 = "Keypad 1";
	static const std::string KEYPAD_2 = "Keypad 2";
	static const std::string KEYPAD_3 = "Keypad 3";
	static const std::string KEYPAD_4 = "Keypad 4";
	static const std::string KEYPAD_5 = "Keypad 5";
	static const std::string KEYPAD_6 = "Keypad 6";
	static const std::string KEYPAD_7 = "Keypad 7";
	static const std::string KEYPAD_8 = "Keypad 8";
	static const std::string KEYPAD_9 = "Keypad 9";
	static const std::string KEYPAD_0 = "Keypad 0";
	static const std::string KEY_BACKSPACE = "Backspace";
	static const std::string KEY_DOT = ".";
	static const std::string KEYPAD_DOT = "Keypad .";

	static const std::map<std::string, std::string> keyToInput =
	{
		{ KEYPAD_0, "0" },
		{ KEYPAD_1, "1" },
		{ KEYPAD_2, "2" },
		{ KEYPAD_3, "3" },
		{ KEYPAD_4, "4" },
		{ KEYPAD_5, "5" },
		{ KEYPAD_6, "6" },
		{ KEYPAD_7, "7" },
		{ KEYPAD_8, "8" },
		{ KEYPAD_9, "9" },
		{ KEY_0, "0" },
		{ KEY_1, "1" },
		{ KEY_2, "2" },
		{ KEY_3, "3" },
		{ KEY_4, "4" },
		{ KEY_5, "5" },
		{ KEY_6, "6" },
		{ KEY_7, "7" },
		{ KEY_8, "8" },
		{ KEY_9, "9" },
		{ KEY_DOT, "." },
		{ KEYPAD_DOT, "." }
	};

	static const std::set<std::string> numericKeys =
	{
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEYPAD_0,
		KEYPAD_1,
		KEYPAD_2,
		KEYPAD_3,
		KEYPAD_4,
		KEYPAD_5,
		KEYPAD_6,
		KEYPAD_7,
		KEYPAD_8,
		KEYPAD_9
	};

	static const std::set<std::string> decimalKeys =
	{
		KEY_DOT,
		KEYPAD_DOT
	};

	static const std::set<std::string> newLineKeys =
	{
		KEY_RETURN,
		KEYPAD_ENTER
	};

	std::function<bool(const std::string&)> Terminal::DoMenuInput(
		const std::string& errorMessage,
		std::function<void()> refresh)
	{
		return [errorMessage, refresh](const std::string& key)
		{
			if (numericKeys.contains(key))
			{
				Terminal::AppendInput(keyToInput.find(key)->second);
				visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, keyToInput.find(key)->second);
				return true;
			}
			else if (newLineKeys.contains(key))
			{
				auto input = Terminal::GetInput();
				if (!input.empty())
				{
					visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
					auto number = common::Data::ToInt(input);
					if (!menu.DoAction(number))
					{
						ErrorMessage(errorMessage);
						refresh();
					}
					return true;
				}
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

	void Terminal::HandleBackspace()
	{
		if (Terminal::Backspace())
		{
			visuals::Terminals::Backspace(LAYOUT_NAME, TERMINAL_ID);
		}
	}

	std::function<bool(const std::string&)> Terminal::DoIntegerInput(
		const std::map<std::string, std::function<void()>>& table,
		std::function<void(const std::string&)> handleOther)
	{
		return [table, handleOther](const std::string& key)
		{
			if (numericKeys.contains(key))
			{
				Terminal::AppendInput(keyToInput.find(key)->second);
				visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, keyToInput.find(key)->second);
				return true;
			}
			else if (newLineKeys.contains(key))
			{
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
				bool result = common::utility::Dispatcher::Dispatch(table, Terminal::GetInput(), true, false);
				if (result)
				{
					Terminal::ClearInput();
				}
				else
				{
					handleOther(Terminal::GetInput());
					Terminal::ClearInput();
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

	std::function<bool(const std::string&)> Terminal::DoIntegerInput(
		const std::map<std::string, std::function<void()>>& table, 
		const std::string& errorMessage, 
		std::function<void()> refresh)
	{
		return DoIntegerInput(
			table, 
			[errorMessage, refresh](const std::string&)
			{
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
				visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::RED);
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, errorMessage);
				visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::GRAY);
				refresh();
			});
	}

	std::function<bool(const std::string&)> Terminal::DoDoubleInput(
		std::function<void()> processInvalidInput, 
		std::function<void(double)> processValidInput)
	{
		return [processInvalidInput, processValidInput](const std::string& key)
		{
			if (numericKeys.contains(key))
			{
				Terminal::AppendInput(keyToInput.find(key)->second);
				visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, keyToInput.find(key)->second);
				return true;
			}
			else if (decimalKeys.contains(key))
			{
				if (GetInput().size() > 0 && GetInput().find('.') == std::string::npos)
				{
					Terminal::AppendInput(keyToInput.find(key)->second);
					visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, keyToInput.find(key)->second);
					return true;
				}
			}
			else if (newLineKeys.contains(key))
			{
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
				if (GetInput().empty())
				{
					processInvalidInput();
				}
				else
				{
					processValidInput(common::Data::ToDouble(GetInput()));
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

	void Terminal::ShowPrompt()
	{
		SetForeground(game::Colors::GRAY);
		WriteLine();
		Write(">");
	}

	static void ShowMessages(int characterId)
	{
		auto character = game::session::Character(characterId);
		auto messages = game::session::character::Messages(characterId).GetAll();
		if (!messages.empty())
		{
			Terminal::WriteLine();
			for (auto message : messages)
			{
				Terminal::SetForeground(message.color);
				Terminal::WriteLine(message.text);
			}
			game::session::character::Messages(characterId).Clear();
		}
	}

	void Terminal::Reinitialize()
	{
		ClearStatusLine();
		ClearInput();
		auto characterId = game::session::Player::TryGetCharacterId();
		if (characterId)
		{
			ShowMessages(characterId.value());
		}
		WriteLine();
	}

	void Terminal::ErrorMessage(const std::string& errorMessage)
	{
		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine();
		Terminal::WriteLine(errorMessage);
	}

}