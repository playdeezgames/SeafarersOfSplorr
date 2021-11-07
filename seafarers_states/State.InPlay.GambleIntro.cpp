#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Statistics.h>
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
		return game::avatar::Docked::ReadLocation().value();
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Gambling:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto ante = game::islands::DarkAlley::GetAnte(ReadLocation()).value();
		auto minimum = game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value();
		Terminal::WriteLine("Yer money: {:.4f}", game::avatar::Statistics::ReadMoney(game::Player::GetAvatarId()));
		Terminal::WriteLine("Minimum bet: {:.4f}", minimum);
		Terminal::WriteLine("Ante: {:.4f}", ante);

		Terminal::SetForeground(game::Colors::YELLOW);
		if (game::avatar::Statistics::ReadMoney(game::Player::GetAvatarId()) >= minimum)
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
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void DealHand()
	{
		if (game::avatar::Statistics::ReadMoney(game::Player::GetAvatarId()) >= game::islands::DarkAlley::GetMinimumWager(ReadLocation()).value())
		{
			game::avatar::Statistics::ChangeMoney(game::Player::GetAvatarId(), -game::islands::DarkAlley::GetAnte(ReadLocation()).value());
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