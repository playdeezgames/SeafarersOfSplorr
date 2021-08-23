#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Confirmations.h>
#include <Visuals.Menus.h>
#include <Visuals.Messages.h>
namespace state::in_play::DarkAlley
{
	const std::string LAYOUT_NAME = "State.InPlay.DarkAlley";
	const std::string MENU_ID = "DarkAlley";

	static void OnLeave()
	{
		visuals::Confirmations::Write(
			{
				"Leave Dark Alley?",
				[]() { game::avatar::Docked::DoAction(game::avatar::Action::ENTER_DOCK); },
				[]() {}
			});
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static double GetMinimumWager()
	{
		return game::islands::DarkAlley::GetMinimumWager(game::avatar::Docked::GetDockedLocation().value()).value();
	}

	const auto GetMoney = game::avatar::Statistics::GetMoney;

	static void OnGamble()
	{
		if (GetMoney() >= GetMinimumWager())
		{
			game::avatar::Docked::DoAction(game::avatar::Action::START_GAMBLING);
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

	enum class DarkAlleyMenuItem
	{
		LEAVE,
		GAMBLE
	};

	const std::map<DarkAlleyMenuItem, std::function<void()>> activators =
	{
		{ DarkAlleyMenuItem::GAMBLE, OnGamble },
		{ DarkAlleyMenuItem::LEAVE, OnLeave }
	};

	const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DARK_ALLEY, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_DARK_ALLEY, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DARK_ALLEY, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandler(::UIState::IN_PLAY_DARK_ALLEY, OnLeave);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DARK_ALLEY, LAYOUT_NAME);
	}
}
