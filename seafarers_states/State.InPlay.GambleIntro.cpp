#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Dispatcher.h>
#include <Data.Game.Island.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Player.h>
#include "State.InPlay.GambleIntro.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_GAMBLE_INTRO;

	static common::XY<double> ReadLocation()
	{
		return data::game::Island::Read(game::character::Docked::ReadLocation().value()).value().location;
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Gambling:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto ante = game::islands::DarkAlley::GetAnte(ReadLocation()).value();
		auto minimum = game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value();
		Terminal::WriteLine("Yer money: {:.4f}", game::character::Statistics::ReadMoney(game::Player::GetCharacterId()));
		Terminal::WriteLine("Minimum bet: {:.4f}", minimum);
		Terminal::WriteLine("Ante: {:.4f}", ante);

		Terminal::SetForeground(game::Colors::YELLOW);
		if (game::character::Statistics::ReadMoney(game::Player::GetCharacterId()) >= minimum)
		{
			Terminal::WriteLine("1) Play a hand");
		}
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnLeave()
	{
		game::character::Actions::DoAction(game::Player::GetCharacterId(), game::character::Action::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void DealHand()
	{
		if (game::character::Statistics::ReadMoney(game::Player::GetCharacterId()) >= game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value())
		{
			game::character::Statistics::ChangeMoney(game::Player::GetCharacterId(), -game::islands::DarkAlley::GetAnte(ReadLocation()).value());
			application::UIState::Write(::UIState::IN_PLAY_GAMBLE_PLAY);
		}
		else
		{
			Terminal::WriteLine(Terminal::INVALID_INPUT);
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", DealHand},
		{ "0", OnLeave}
	};

	void GambleIntro::Start()
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