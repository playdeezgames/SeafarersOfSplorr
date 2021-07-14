#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Texts.h"
#include "Data.Stores.h"
#include "Game.Avatar.h"
#include "Game.Islands.h"
#include <format>
namespace state::in_play::IslandJobs
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandJobs";
	const std::string MENU_ID = "AcceptJob";

	enum class AcceptJobMenuItem
	{
		ACCEPT,
		CANCEL
	};

	static void OnAccept()
	{
		return;
	}

	const std::map<AcceptJobMenuItem, std::function<void()>> activators =
	{
		{ AcceptJobMenuItem::ACCEPT, OnAccept },
		{ AcceptJobMenuItem::CANCEL, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (AcceptJobMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		auto location = game::Avatar::GetDockedLocation().value();
		auto island = game::Islands::Read(location).value();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_JOBS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_JOBS, LAYOUT_NAME);
	}
}