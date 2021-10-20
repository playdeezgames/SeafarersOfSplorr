#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Common.Data.h>
#include <format>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Log.h>
#include <Game.Colors.h>
#include <Game.Ship.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state::in_play//20211020
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_SPEED;

	static const std::string LAYOUT_NAME = "State.InPlay.ChangeSpeed";
	static const std::string MENU_ID = "ChangeSpeed";

	enum class ChangeSpeedItem
	{
		ALL_STOP,
		AHEAD_ONE_THIRD,
		AHEAD_TWO_THIRDS,
		AHEAD_FULL,
		AHEAD_FLANK,
		BELAY
	};

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT);

	static const double SPEED_ALL_STOP = 0.0;
	static const double SPEED_ONE_THIRD = 0.3;
	static const double SPEED_TWO_THIRDS = 0.6;
	static const double SPEED_FULL = 0.9;
	static const double SPEED_FLANK = 1.0;

	static std::function<void()> DoSetSpeed(double speed)
	{
		return [speed]()
		{
			game::avatar::Log::Write(
				game::Colors::GRAY,
				"Set speed to{:.0f} % !", 
				common::Data::ToPercentage(speed, SPEED_FLANK).value());
			game::Ship::SetSpeed(speed);
			OnLeave();
		};
	}

	static const std::map<ChangeSpeedItem, std::function<void()>> activators =
	{
		{ ChangeSpeedItem::ALL_STOP, DoSetSpeed(SPEED_ALL_STOP) },
		{ ChangeSpeedItem::AHEAD_ONE_THIRD, DoSetSpeed(SPEED_ONE_THIRD) },
		{ ChangeSpeedItem::AHEAD_TWO_THIRDS, DoSetSpeed(SPEED_TWO_THIRDS) },
		{ ChangeSpeedItem::AHEAD_FULL, DoSetSpeed(SPEED_FULL) },
		{ ChangeSpeedItem::AHEAD_FLANK, DoSetSpeed(SPEED_FLANK) },
		{ ChangeSpeedItem::BELAY, ::application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	void ChangeSpeed::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			game::audio::Mux::GoToTheme(
				game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMenuMouseButtonUp(
				LAYOUT_NAME, 
				ActivateItem));
		::application::MouseMotion::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMenuMouseMotion(
				LAYOUT_NAME));
		::application::Command::SetHandlers(
			CURRENT_STATE, 
			commandHandlers);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
	}
}