#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Docked.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
namespace state::in_play::DarkAlley
{
	const std::string LAYOUT_NAME = "State.InPlay.DarkAlley";
	const std::string MENU_ID = "DarkAlley";


	static void OnLeave()
	{
		game::avatar::Docked::DoDockedAction(game::avatar::DockedAction::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnGamble()
	{

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
