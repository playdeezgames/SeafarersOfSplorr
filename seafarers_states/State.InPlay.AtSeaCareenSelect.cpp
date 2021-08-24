#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Islands.h>
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play::AtSeaCareenSelect
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_CAREEN_SELECT;
	static const std::string LAYOUT_NAME = "State.InPlay.AtSeaCareenSelect";
	static const std::string MENU_ID = "Careen";

	enum class CareenMenuItem
	{
		STARBOARD,
		PORT,
		BELAY
	};

	static void OnStarboard()
	{
	}

	static void OnPort()
	{
	}

	static void OnBelay()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<CareenMenuItem, std::function<void()>> activators =
	{
		{ CareenMenuItem::STARBOARD, OnStarboard },
		{ CareenMenuItem::PORT, OnPort },
		{ CareenMenuItem::BELAY, OnBelay }
	};

	const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnBelay },
		{::Command::RED, OnBelay }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}