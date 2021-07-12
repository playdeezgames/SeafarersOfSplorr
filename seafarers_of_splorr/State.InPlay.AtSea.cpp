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
#include "Game.Heading.h"
#include "Game.Avatar.Statistics.h"
namespace state::in_play::AtSea
{
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";
	const std::string TEXT_AVATAR_TURNS = "avatar-turns";
	const std::string TEXT_AVATAR_HEALTH = "avatar-health";
	const std::string TEXT_AVATAR_HUNGER = "avatar-hunger";
	const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	const std::string TEXT_AVATAR_SPEED = "avatar-speed";
	const std::string MENU_ID = "Order";
	const std::string MENU_ITEM_DOCK = "Dock";
	const std::string AREA_CHANGE_HEADING = "ChangeHeading";
	const std::string AREA_CHANGE_SPEED = "ChangeSpeed";
	const std::string AREA_MOVE = "Move";
	const std::string FORMAT_TURNS = "Turns Left: {:.0f}";
	const std::string FORMAT_HUNGER = "Hunger: {:.0f}";
	const std::string FORMAT_HEALTH = "Health: {:.0f}";
	const std::string FORMAT_HEADING = "Heading: {:.2f}";
	const std::string FORMAT_SPEED = "Speed: {:.2f}";
	const std::string IMAGE_CURRENT_HEADING = "CurrentHeading";
	const std::string IMAGE_NEW_HEADING = "NewHeading";
	const std::string IMAGE_DESTINATION = "Destination";
	const common::XY<int> CENTER = { 82, 82 };

	static double newHeading = 0.0;

	enum class OrderMenuItem
	{
		CHANGE_SPEED,
		HEAD_FOR,
		MOVE,
		DOCK
	};

	static void OnChangeSpeed()
	{
		::application::UIState::Write(::UIState::IN_PLAY_CHANGE_SPEED);
	}

	static void OnEnter();
	static void OnMove()
	{
		game::Avatar::Move();
		application::UIState::EnterGame();
		OnEnter();
	}

	static void OnDock()
	{
		game::Avatar::Dock();
		application::UIState::EnterGame();
	}

	static void OnHeadFor()
	{
		application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR);
	}

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{OrderMenuItem::CHANGE_SPEED, OnChangeSpeed},
		{OrderMenuItem::MOVE, OnMove},
		{OrderMenuItem::DOCK, OnDock},
		{OrderMenuItem::HEAD_FOR, OnHeadFor}
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

	static void UpdateAvatarHunger()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HUNGER, std::format(FORMAT_HUNGER, game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HUNGER)));
	}

	static void UpdateAvatarHealth()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEALTH, std::format(FORMAT_HEALTH, game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HEALTH)));
	}

	static void UpdateAvatarTurns()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_TURNS, std::format(FORMAT_TURNS, game::avatar::Statistics::GetCurrent(game::avatar::Statistic::TURNS_REMAINING)));
	}

	static void UpdateAvatarHeading()
	{
		auto heading = game::Avatar::GetHeading();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEADING, std::format(FORMAT_HEADING, heading));
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_CURRENT_HEADING, heading);
	}

	static void UpdateAvatarSpeed()
	{
		auto speed = game::Avatar::GetSpeed();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format(FORMAT_SPEED, speed));
	}

	static common::XY<double> Plot(const common::XY<double>&);
	static void UpdateAvatarDestination()
	{
		auto destination = game::Avatar::GetDestination();
		if (destination)
		{
			auto plot = Plot(destination.value() - game::Avatar::GetLocation());
			visuals::Images::SetLocation(LAYOUT_NAME, IMAGE_DESTINATION, { (int)plot.GetX(), (int)plot.GetY() });
			visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_DESTINATION, true);
			return;
		}
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_DESTINATION, false);
	}

	static void UpdateAvatarStatus()
	{
		UpdateAvatarHeading();
		UpdateAvatarHealth();
		UpdateAvatarHunger();
		UpdateAvatarSpeed();
		UpdateAvatarTurns();
		UpdateAvatarDestination();
	}

	//TODO: get this not hardcoded
	const common::XY<double> VIEW_CENTER = { 162.0,182.0 };
	const double VIEW_RADIUS = 144.0;
	const int ISLAND_ICON_COUNT = 10;
	const int IMAGE_OFFSET_X = 0;
	const int IMAGE_OFFSET_Y = 0;
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

	static common::XY<double> Plot(const common::XY<double>& location)
	{
		double viewScale = VIEW_RADIUS / game::World::GetViewDistance();
		return location* viewScale + VIEW_CENTER;
	}

	static void UpdateIslands()
	{
		HideVisibleIslands();
		auto islands = game::Islands::GetViewableIslands();
		int icon = 0;
		double dockDistance = game::World::GetDockDistance();
		bool canDock = false;
		for (auto& entry : islands)
		{
			canDock |= (game::Heading::Distance(entry.relativeLocation, { 0.0, 0.0 }) <= dockDistance);
			auto plot = Plot(entry.relativeLocation);
			auto visualId = std::format("AtSeaIsland{}", icon);
			visuals::Images::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + IMAGE_OFFSET_X,(int)plot.GetY() + IMAGE_OFFSET_Y });
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, true);
			visuals::Texts::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + TEXT_OFFSET_X,(int)plot.GetY() + TEXT_OFFSET_Y });
			visuals::Texts::SetText(LAYOUT_NAME, visualId, (entry.visits.has_value()) ? (entry.name) : ("????"));
			++icon;
		}
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_DOCK, canDock);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateAvatarStatus();
		UpdateIslands();
	}


	static void OnMouseMotionInHelm(const common::XY<int>& location)
	{
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, true);
		auto delta = location - CENTER;
		newHeading = game::Heading::XYToDegrees({(double)delta.GetX(), (double)delta.GetY()});
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_NEW_HEADING, newHeading);
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		if (areaName == AREA_CHANGE_HEADING)
		{
			OnMouseMotionInHelm(location);
			return;
		}
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, false);
	}

	static void OnMouseMotiionOutsideArea(const common::XY<int>& location)
	{
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, false);
	}

	static bool OnMouseButtonUp(const std::string& areaName)
	{
		if (areaName == AREA_CHANGE_HEADING)
		{
			game::Avatar::SetHeading(newHeading);
			UpdateAvatarStatus();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotiionOutsideArea));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUp));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
	}
}