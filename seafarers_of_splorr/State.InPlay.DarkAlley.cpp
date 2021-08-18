#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Confirmations.h"
#include "Game.Islands.DarkAlley.h"
#include "Game.Messages.h"
#include "UIState.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.Menus.h"
namespace state::in_play::DarkAlley
{
	const std::string LAYOUT_NAME = "State.InPlay.DarkAlley";
	const std::string MENU_ID = "DarkAlley";

	static void OnLeave()
	{
		visuals::Confirmations::Write(
			{
				"Leave Dark Alley?",
				[]() { game::avatar::Docked::DoDockedAction(game::avatar::DockedAction::ENTER_DOCK); },
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
			return;
		}
		visuals::Messages::Send(
			{
				"Come Back When Yer Serious!",
				{
					{
						{19,9},
						"You don't have enough money!",
						visuals::data::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,11},
						std::format("Minimum bet: {:.4f}", GetMinimumWager()),
						visuals::data::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					}
				}
			});
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
