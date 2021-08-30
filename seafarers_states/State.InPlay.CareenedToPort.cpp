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
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Islands.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CAREENED_TO_PORT;
	static const std::string LAYOUT_NAME = "State.InPlay.CareenedToPort";
	static const std::string MENU_ID = "Careen";

	enum class CareenMenuItem
	{
		CLEAN_HULL,
		WEIGH_ANCHOR
	};

	static void OnCleanHull()
	{
		game::avatar::ShipStatistics::CleanHull(game::Side::STARBOARD);
	}

	static void OnWeighAnchor()
	{
		game::Avatar::DoAction(game::avatar::Action::UNCAREEN);
		application::UIState::Write(::UIState::IN_PLAY_AT_SEA_CAREEN_SELECT);
	}

	const std::map<CareenMenuItem, std::function<void()>> activators =
	{
		{ CareenMenuItem::CLEAN_HULL, OnCleanHull },
		{ CareenMenuItem::WEIGH_ANCHOR, OnWeighAnchor },
	};

	const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnWeighAnchor },
		{::Command::RED, OnWeighAnchor }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	void CareenedToPort::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}