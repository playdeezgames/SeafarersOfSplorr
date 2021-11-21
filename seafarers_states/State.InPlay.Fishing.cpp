#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Items.h>
#include <Game.Colors.h>
#include <Game.Fishboard.h>
#include <Game.FishGame.h>
#include <Game.Player.h>
#include "State.InPlay.Fishing.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_FISHING;
	static const int COLUMNS = 5;
	static const int ROWS = 5;

	static const std::string CELL_TOP = "\xda\xc4\xc4\xbf";
	static const std::string CELL_MIDDLE = "\xb3{:2s}\xb3";
	static const std::string CELL_BOTTOM = "\xc0\xc4\xc4\xd9";

	static void RefreshCellTop(int row, int column)
	{
		Terminal::Write(CELL_TOP);
	}

	static void RefreshRowTop(int row)
	{
		for (int column = 0; column < COLUMNS; ++column)
		{
			RefreshCellTop(row, column);
		}
		Terminal::WriteLine();
	}

	static const std::map<std::optional<game::Fish>, std::string> cellContents =
	{
		{std::nullopt, "  "},
		{game::Fish::COD, "CO"},
		{game::Fish::MARLIN, "MA"},
		{game::Fish::SEAWEED, "SW"},
		{game::Fish::TROUSERS, "TR"},
		{game::Fish::TUNA, "TU"}
	};

	static void RefreshCellMiddle(int row, int column)
	{
		auto cell = game::Fishboard::Read({ column, row });
		if (
			cell.revealed || 
			game::FishGame::HasGivenUp() || 
			game::FishGame::GetState() == game::FishGameState::DONE)
		{
			Terminal::Write(CELL_MIDDLE, cellContents.find(cell.fish)->second);
		}
		else
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::Write("\xb3");
			Terminal::SetForeground(game::Colors::YELLOW);
			Terminal::Write("{:2d}", column + row * COLUMNS + 1);
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::Write("\xb3");
		}
	}

	static void RefreshRowMiddle(int row)
	{
		for (int column = 0; column < COLUMNS; ++column)
		{
			RefreshCellMiddle(row, column);
		}
		Terminal::WriteLine();
	}

	static void RefreshCellBottom(int row, int column)
	{
		Terminal::Write(CELL_BOTTOM);
	}

	static void RefreshRowBottom(int row)
	{
		for (int column = 0; column < COLUMNS; ++column)
		{
			RefreshCellBottom(row, column);
		}
		Terminal::WriteLine();
	}

	static void RefreshBoard()
	{
		Terminal::SetForeground(game::Colors::GRAY);
		for (int row = 0; row < ROWS; ++row)
		{
			RefreshRowTop(row);
			RefreshRowMiddle(row);
			RefreshRowBottom(row);
		}
	}

	static const std::map<game::FishGameState, std::string> goBackMenuItemTexts =
	{
		{game::FishGameState::DONE, "Done!"},
		{game::FishGameState::GAVE_UP, "Never mind"},
		{game::FishGameState::FISHING, "Give Up!"},
		{game::FishGameState::OUT_OF_GUESSES, "Give Up!"}
	};

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Fishing:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Guesses Left: {} Completion: {:.0f}%", game::FishGame::ReadGuesses(), game::Fishboard::ReadProgressPercentage());
		RefreshBoard();

		Terminal::SetForeground(game::Colors::YELLOW);
		if (game::FishGame::GetState() == 
			game::FishGameState::OUT_OF_GUESSES && 
			game::avatar::Items::Has(
				game::Player::GetAvatarId(), 
				game::Item::BAIT))
		{
			Terminal::WriteLine("1) Use more bait");
		}
		Terminal::WriteLine("0) {}", goBackMenuItemTexts.find(game::FishGame::GetState())->second);

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		Refresh();
	}

	static void GiveUp()
	{
		game::FishGame::GiveUp();
		Refresh();
	}

	static void LeaveFishing()
	{
		game::avatar::Actions::DoAction(game::Player::GetAvatarId(), game::avatar::Action::STOP_FISHING);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<game::FishGameState, std::function<void()>> leaveHandlers =
	{
		{game::FishGameState::DONE, LeaveFishing },
		{game::FishGameState::GAVE_UP, LeaveFishing },
		{game::FishGameState::FISHING, GiveUp},
		{game::FishGameState::OUT_OF_GUESSES, GiveUp }
	};

	static void OnLeave()
	{
		common::utility::Dispatcher::Dispatch(leaveHandlers, game::FishGame::GetState());
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave }
	};

	static bool FishingInputHandler(const std::string& line)
	{
		int index = common::Data::ToInt(line)-1;
		if (index >= 0 && index < COLUMNS * ROWS)
		{
			int column = index % COLUMNS;
			int row = index / COLUMNS;
			auto cell = game::Fishboard::Read({ column, row });
			if (!cell.revealed)
			{
				game::Fishboard::Reveal({column, row});
				Refresh();
				return true;
			}
		}
		return false;
	}

	static bool OutOfGuessesInputHandler(const std::string& line)
	{
		if (line == "1" && game::avatar::Items::Has(game::Player::GetAvatarId(), game::Item::BAIT))
		{
			game::FishGame::AddBait();
			Refresh();
			return true;
		}
		return false;
	}

	static const std::map<game::FishGameState, std::function<bool(const std::string&)>> otherInputHandlers =
	{
		{game::FishGameState::DONE, [](const std::string&) { return false; }},
		{game::FishGameState::FISHING, FishingInputHandler},
		{game::FishGameState::GAVE_UP, [](const std::string&) { return false; }},
		{game::FishGameState::OUT_OF_GUESSES, OutOfGuessesInputHandler}
	};

	static void OnOtherInput(const std::string& line)
	{
		if (!common::utility::Dispatcher::DispatchParameter(
			otherInputHandlers, 
			game::FishGame::GetState(), 
			line, 
			false))
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void Fishing::Start()
	{
		application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}