#include <Common.Data.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Session.h>
#include "State.InPlay.Fishing.h"
#include "State.InPlay.Globals.h"
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
		auto fishGame = game::Session().GetPlayerCharacter().GetFishGame();
		auto cell = fishGame.GetBoard().value().GetCell(column, row);
		if (
			cell.IsRevealed() || 
			fishGame.HasGivenUp().value() || 
			fishGame.GetState().value() == game::FishGameState::DONE)
		{
			Terminal::Write(CELL_MIDDLE, cellContents.find(cell.GetFish())->second);
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
		auto fishGame = game::Session().GetPlayerCharacter().GetFishGame();

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Fishing:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Guesses Left: {} Completion: {:.0f}%", 
			fishGame.GetGuesses().value(), 
			fishGame.GetBoard().value().GetProgressPercentage());
		RefreshBoard();

		auto items = game::Session().GetPlayerCharacter().GetItems();

		Terminal::SetForeground(game::Colors::YELLOW);
		if (fishGame.GetState().value() ==
			game::FishGameState::OUT_OF_GUESSES && 
			items.Has(game::Item::BAIT))
		{
			Terminal::WriteLine("1) Use more bait");
		}
		Terminal::WriteLine("0) {}", goBackMenuItemTexts.find(fishGame.GetState().value())->second);

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		Refresh();
	}

	static void GiveUp()
	{
		game::Session().GetPlayerCharacter().GetFishGame().GiveUp();
		Refresh();
	}

	static void LeaveFishing()
	{
		game::Session().GetPlayerCharacter().DoAction(game::characters::Action::STOP_FISHING);
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
		common::utility::Dispatcher::Dispatch(leaveHandlers, 
			game::Session().GetPlayerCharacter().GetFishGame().GetState().value());
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
			auto cell = game::Session().GetPlayerCharacter().GetFishGame().GetBoard().value().GetCell(column, row);
			if (!cell.IsRevealed())
			{
				cell.Reveal();
				Refresh();
				return true;
			}
		}
		return false;
	}

	static bool OutOfGuessesInputHandler(const std::string& line)
	{
		auto playerCharacter = game::Session().GetPlayerCharacter();
		if (line == "1" && playerCharacter.GetItems().Has(game::Item::BAIT))
		{
			playerCharacter.GetFishGame().AddBait();
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
			game::Session().GetPlayerCharacter().GetFishGame().GetState().value(),
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