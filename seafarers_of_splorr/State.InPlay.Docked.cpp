#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Islands.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Visuals.Texts.h"
namespace state::in_play::Docked
{
	const std::string LAYOUT_NAME = "State.InPlay.Docked";
	const std::string MENU_ID = "Order";
	const std::string TEXT_ISLAND_NAME = "island-name";
	const std::string TEXT_ISLAND_VISITS = "island-visits";
	const std::string FORMAT_NAME = "Name: {}";
	const std::string FORMAT_VISITS = "Visits: {}";

	enum class OrderMenuItem
	{
		UNDOCK,
		JOBS,
		TRADE
	};

	static void OnUndock()
	{
		game::Avatar::Undock();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{ OrderMenuItem::UNDOCK, OnUndock },
		{ OrderMenuItem::JOBS, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_JOBS) },
		{ OrderMenuItem::TRADE, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_TRADE) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (OrderMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnUndock },
		{::Command::RED, OnUndock }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		auto location = game::Avatar::GetDockedLocation().value();
		auto island = game::Islands::Read(location).value();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ISLAND_NAME, std::format(FORMAT_NAME,island.name));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ISLAND_VISITS, std::format(FORMAT_VISITS, island.visits.value_or(0)));
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DOCKED, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_DOCKED, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DOCKED, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_DOCKED, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DOCKED, LAYOUT_NAME);
	}
}