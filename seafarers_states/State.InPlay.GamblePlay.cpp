#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Audio.Sfx.h>
#include <Game.Character.Docked.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.DarkAlley.GamblingHand.h>
#include <Game.Player.h>
#include "State.InPlay.DarkAlleyEntrance.h"
#include "State.InPlay.GamblePlay.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_GAMBLE_PLAY;

	static common::XY<double> ReadLocation()
	{
		return game::character::Docked::ReadLocation().value();
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Play Hand:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Yer money: {:.4f}", game::avatar::Statistics::ReadMoney(game::Player::GetAvatarId()));
		Terminal::WriteLine("Minimum wager(less ante): {:.4f}", 
			game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value() - 
			game::islands::DarkAlley::GetAnte(ReadLocation()).value());
		auto hand = game::islands::dark_alley::GamblingHand::Read(ReadLocation());

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::Write("First Card: ");
		Terminal::SetForeground(DarkAlleyEntrance::SuitColors.find(std::get<1>(hand[0]))->second);
		Terminal::Write(DarkAlleyEntrance::Ranks.find(std::get<0>(hand[0]))->second);
		Terminal::Write(DarkAlleyEntrance::Suits.find(std::get<1>(hand[0]))->second);
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::Write("Second Card: ");
		Terminal::SetForeground(DarkAlleyEntrance::SuitColors.find(std::get<1>(hand[1]))->second);
		Terminal::Write(DarkAlleyEntrance::Ranks.find(std::get<0>(hand[1]))->second);
		Terminal::Write(DarkAlleyEntrance::Suits.find(std::get<1>(hand[1]))->second);
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
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		game::islands::dark_alley::GamblingHand::Deal(ReadLocation());
		Refresh();
	}

	static void OnPlaceMinimumWager()
	{
		auto hand = game::islands::dark_alley::GamblingHand::Read(ReadLocation());
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::Write("Final Card: ");
		Terminal::SetForeground(DarkAlleyEntrance::SuitColors.find(std::get<1>(hand[2]))->second);
		Terminal::Write(DarkAlleyEntrance::Ranks.find(std::get<0>(hand[2]))->second);
		Terminal::Write(DarkAlleyEntrance::Suits.find(std::get<1>(hand[2]))->second);
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GRAY);
		if (game::islands::dark_alley::GamblingHand::IsWinner(ReadLocation()))
		{
			game::audio::Sfx::Play(game::audio::GameSfx::WOOHOO);
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You win!");
			game::avatar::Statistics::ChangeMoney(game::Player::GetAvatarId(), game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value() + game::islands::DarkAlley::GetAnte(ReadLocation()).value());
		}
		else
		{
			game::audio::Sfx::Play(game::audio::GameSfx::SHUCKS);
			Terminal::SetForeground(game::Colors::RED);
			Terminal::WriteLine("You lose!");
			game::avatar::Statistics::ChangeMoney(game::Player::GetAvatarId(), -game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value() + game::islands::DarkAlley::GetAnte(ReadLocation()).value());
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