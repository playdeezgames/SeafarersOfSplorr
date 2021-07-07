#include <string>
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Game.Avatar.h"
#include "Visuals.Texts.h"
#include <format>
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Application.MouseMotion.h"
#include "Application.MouseButtonUp.h"
#include "Game.Islands.h"
#include "Visuals.Images.h"
#include "Game.World.h"
namespace state::in_play::AtSea
{
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";
	const std::string TEXT_AVATAR_X = "avatar-x";
	const std::string TEXT_AVATAR_Y = "avatar-y";
	const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	const std::string TEXT_AVATAR_SPEED = "avatar-speed";
	const std::string MENU_ID = "Order";
	const std::string AREA_CHANGE_HEADING = "ChangeHeading";
	const std::string AREA_CHANGE_SPEED = "ChangeSpeed";
	const std::string AREA_MOVE = "Move";
	const std::string FORMAT_X = "X: {:.2f}";
	const std::string FORMAT_Y = "Y: {:.2f}";
	const std::string FORMAT_HEADING = "Heading: {:.2f}";
	const std::string FORMAT_SPEED = "Speed: {:.2f}";
	const std::string IMAGE_CURRENT_HEADING = "CurrentHeading";

	enum class OrderMenuItem
	{
		CHANGE_HEADING,
		CHANGE_SPEED,
		MOVE
	};

	static void OnChangeHeading()
	{
		::application::UIState::Write(::UIState::IN_PLAY_CHANGE_HEADING);
	}

	static void OnChangeSpeed()
	{
		::application::UIState::Write(::UIState::IN_PLAY_CHANGE_SPEED);
	}

	static void OnEnter();
	static void OnMove()
	{
		game::Avatar::Move();
		OnEnter();
	}

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{OrderMenuItem::CHANGE_HEADING, OnChangeHeading},
		{OrderMenuItem::CHANGE_SPEED, OnChangeSpeed},
		{OrderMenuItem::MOVE, OnMove}
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
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	static void UpdateAvatarStatus()
	{
		auto location = game::Avatar::GetLocation();
		auto heading = game::Avatar::GetHeading();
		auto speed = game::Avatar::GetSpeed();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_X, std::format(FORMAT_X, location.GetX()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_Y, std::format(FORMAT_Y, location.GetY()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEADING, std::format(FORMAT_HEADING, heading));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format(FORMAT_SPEED, speed));
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_CURRENT_HEADING, heading);
	}

	//TODO: get this not hardcoded
	const common::XY<double> VIEW_CENTER = { 170.0,190.0 };
	const double VIEW_RADIUS = 144.0;
	const int ISLAND_ICON_COUNT = 10;
	const int IMAGE_OFFSET_X = -8;
	const int IMAGE_OFFSET_Y = -8;
	const int TEXT_OFFSET_X = 0;
	const int TEXT_OFFSET_Y = 8;

	static void HideVisibleIslands()
	{
		for (int icon = 0; icon < ISLAND_ICON_COUNT; ++icon)
		{
			auto visualId = std::format("AtSeaIsland{}", icon);
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, false);
			visuals::Texts::SetText(LAYOUT_NAME, visualId, "");
		}
	}

	static void UpdateIslands()
	{
		HideVisibleIslands();
		auto islands = game::Islands::GetViewableIslands();
		int icon = 0;
		double viewScale = VIEW_RADIUS / game::World::GetViewDistance();
		for (auto& entry : islands)
		{
			auto plot = entry.location * viewScale + VIEW_CENTER;
			auto visualId = std::format("AtSeaIsland{}", icon);
			visuals::Images::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + IMAGE_OFFSET_X,(int)plot.GetY() + IMAGE_OFFSET_Y });
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, true);
			visuals::Texts::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + TEXT_OFFSET_X,(int)plot.GetY() + TEXT_OFFSET_Y });
			visuals::Texts::SetText(LAYOUT_NAME, visualId, (entry.visits.has_value()) ? (entry.name) : ("????"));
			++icon;
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateAvatarStatus();
		UpdateIslands();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
	}
}