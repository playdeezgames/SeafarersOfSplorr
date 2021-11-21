#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Player.h>
#include "State.InPlay.DarkAlley.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Confirmations.h>
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DARK_ALLEY;

	static void OnLeave()
	{
		visuals::Confirmations::Write(
			{
				"Leave Dark Alley?",
				[]() { game::avatar::Actions::DoAction(game::Player::GetAvatarId(), game::avatar::Action::ENTER_DOCK); },
				[]() {}
			});
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static double GetMinimumWager()
	{
		return game::islands::DarkAlley::GetMinimumWager(game::avatar::Docked::ReadLocation().value()).value();
	}

	static const auto ReadMoney = game::avatar::Statistics::ReadMoney;

	static void OnGamble()
	{
		if (ReadMoney(game::Player::GetAvatarId()) >= GetMinimumWager())
		{
			game::avatar::Actions::DoAction(game::Player::GetAvatarId(), game::avatar::Action::START_GAMBLING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		visuals::Messages::Write(
			{
				"Come Back When Yer Serious!",
				{
					{
						{19,9},
						"You don't have enough money!",
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,11},
						std::format("Minimum bet: {:.4f}", GetMinimumWager()),
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					}
				}
			});
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dark Alley:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Gamble");
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}
	
	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnGamble},
		{ "0", OnLeave}
	};

	void DarkAlley::Start()
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
