#include <Game.Characters.Statistics.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.DarkAlley.GamblingHand.h>
#include "State.InPlay.DarkAlleyEntrance.h"
#include "State.InPlay.GamblePlay.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_GAMBLE_PLAY;

	static std::vector<cards::Card> GetHand()
	{
		return game::islands::dark_alley::GamblingHand::Read(GetPlayerCharacterIslandId().value());
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Play Hand:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Yer money: {:.4f}", GetPlayerCharacterMoney().value());
		Terminal::WriteLine("Minimum wager(less ante): {:.4f}", 
			GetPlayerCharacterDarkAlleyMinimumWager().value() -
			GetPlayerCharacterDarkAlleyAnte().value());
		auto hand = GetHand();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::Write("First Card: ");
		Terminal::SetForeground(GetSuitColors().find(std::get<1>(hand[0]))->second);
		Terminal::Write(GetRanks().find(std::get<0>(hand[0]))->second);
		Terminal::Write(GetSuits().find(std::get<1>(hand[0]))->second);
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::Write("Second Card: ");
		Terminal::SetForeground(GetSuitColors().find(std::get<1>(hand[1]))->second);
		Terminal::Write(GetRanks().find(std::get<0>(hand[1]))->second);
		Terminal::Write(GetSuits().find(std::get<1>(hand[1]))->second);
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Is the next card between these two?");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Bet minimum wager");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		game::islands::dark_alley::GamblingHand::Deal(GetPlayerCharacterIslandId().value());
		Refresh();
	}

	static void OnPlaceMinimumWager()
	{
		auto hand = GetHand();
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::Write("Final Card: ");
		Terminal::SetForeground(GetSuitColors().find(std::get<1>(hand[2]))->second);
		Terminal::Write(GetRanks().find(std::get<0>(hand[2]))->second);
		Terminal::Write(GetSuits().find(std::get<1>(hand[2]))->second);
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GRAY);
		if (game::islands::dark_alley::GamblingHand::IsWinner(GetPlayerCharacterIslandId().value()))
		{
			game::audio::Sfx::Play(game::audio::GameSfx::WOOHOO);
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You win!");
			ChangePlayerCharacterMoneyLegacy(GetPlayerCharacterDarkAlleyMinimumWager().value() + GetPlayerCharacterDarkAlleyAnte().value());
		}
		else
		{
			game::audio::Sfx::Play(game::audio::GameSfx::SHUCKS);
			Terminal::SetForeground(game::Colors::RED);
			Terminal::WriteLine("You lose!");
			ChangePlayerCharacterMoneyLegacy(-GetPlayerCharacterDarkAlleyMinimumWager().value() + GetPlayerCharacterDarkAlleyAnte().value());
		}
		Terminal::SetForeground(game::Colors::GRAY);
		application::UIState::Write(::UIState::IN_PLAY_GAMBLE_INTRO);//TODO: play again y/n?
	}

	static void OnLeave()
	{
		Terminal::WriteLine(game::Colors::CYAN);
		Terminal::WriteLine("You decide not to risk yer money.");
		application::UIState::Write(::UIState::IN_PLAY_GAMBLE_INTRO);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnPlaceMinimumWager},
		{ "0", OnLeave}
	};

	void GamblePlay::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}