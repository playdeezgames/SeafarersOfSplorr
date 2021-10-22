#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.Statistics.h>
#include <Game.BerthType.h>
#include <Game.Colors.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include <Game.Player.h>
#include <Game.Ship.Crew.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Buttons.h>
#include <Visuals.SpriteGrid.h>
namespace state::in_play//20211019
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_LIST;
	static const std::string LAYOUT_NAME = "State.InPlay.CrewList";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string BUTTON_GO_BACK = "GoBack";
	static const std::string AREA_GO_BACK = "GoBack";
	static const std::string AREA_LIST = "List";

	static const auto WriteTextToGrid = 
		visuals::SpriteGrid::DoWriteToGrid(
			LAYOUT_NAME, 
			SPRITE_GRID_ID, 
			FONT_DEFAULT, 
			visuals::HorizontalAlignment::LEFT);

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS);

	struct RosterItem
	{
		std::string name;
		std::string berth;
		std::string mark;
		int avatarId;
	};

	static std::vector<RosterItem> rosterItems;
	static std::optional<size_t> rosterIndex = std::nullopt;

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", game::Colors::YELLOW);
		WriteTextToGrid({ 37, 0 }, "->", game::Colors::YELLOW);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format(" {:15s}   {:15s}", "Name", "Berth"), game::Colors::YELLOW);
	}

	static void RefreshRoster()
	{
		size_t index = 0;
		for (auto& rosterItem : rosterItems)
		{
			auto color = (rosterIndex.has_value() && rosterIndex.value() == index) ? (game::Colors::CYAN) : (game::Colors::GRAY);
			WriteTextToGrid({ 0,2+(int)index }, std::format("{:1s}{:15s}   {:15s}",rosterItem.mark, rosterItem.name, rosterItem.berth), color);
			index++;
		}
	}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshRoster();
	}

	static const std::map<game::BerthType, std::string> berthNames =
	{
		{game::BerthType::CAPTAIN, "Captain"},
		{game::BerthType::CAPTIVE, "Captive"},
		{game::BerthType::CREW, "Crew"},
		{game::BerthType::OFFICER, "Officer"},
		{game::BerthType::PASSENGER, "Passenger"}
	};

	static void UpdateRoster()
	{
		rosterItems.clear();
		auto crew = game::ship::Crew::Read();
		for (auto& entry : crew)
		{
			rosterItems.push_back({
				entry.name,
				berthNames.find(entry.berthType)->second,
				(entry.avatarId==game::Player::GetAvatarId()) ? ("*") : (" "),
				entry.avatarId
				});
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateRoster();
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void OnHoverGoBack(const common::XY<int>&)
	{
		visuals::Buttons::SetHoverButton(LAYOUT_NAME, BUTTON_GO_BACK);
	}

	static void OnHoverList(const common::XY<int>& location)
	{
		size_t row = (size_t)(location.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_ID));
		if (row < rosterItems.size())
		{
			rosterIndex = row;
		}
		else
		{
			rosterIndex = std::nullopt;
		}
	}

	static const std::map<std::string, std::function<void(const common::XY<int>&)>> motionAreas =
	{
		{AREA_GO_BACK, OnHoverGoBack},
		{AREA_LIST, OnHoverList}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		visuals::Buttons::ClearHoverButton(LAYOUT_NAME);
		common::utility::Dispatcher::DispatchParameter(motionAreas, areaName, location);
		Refresh();
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Buttons::ClearHoverButton(LAYOUT_NAME);
		rosterIndex = std::nullopt;
		Refresh();
	}

	static void OnCrewDetail()
	{
		if (rosterIndex)
		{
			CrewDetail::SetAvatarId(rosterItems[rosterIndex.value()].avatarId);
			application::UIState::Write(::UIState::IN_PLAY_CREW_DETAIL);
		}
	}

	static const std::map<std::string, std::function<void()>> buttonUpAreas =
	{
		{AREA_GO_BACK, OnLeave},
		{AREA_LIST, OnCrewDetail}
	};

	void CrewList::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::MouseButtonUp::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMouseButtonUp(
				LAYOUT_NAME, 
				common::utility::Dispatcher::DoDispatch(buttonUpAreas, true, false)));
		::application::MouseMotion::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMouseMotion(
				LAYOUT_NAME, 
				OnMouseMotionInArea, 
				OnMouseMotionOutsideAreas));
		::application::Command::SetHandlers(
			CURRENT_STATE, 
			commandHandlers);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
	}
}