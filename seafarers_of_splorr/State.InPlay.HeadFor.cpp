#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Application.Update.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Destination.h"
#include "Game.Islands.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
#include "Visuals.WorldMap.h"
namespace state::in_play::HeadFor
{
	const std::string LAYOUT_NAME = "State.InPlay.HeadFor";

	const std::string AREA_WORLD_MAP = "WorldMap";
	const std::string AREA_SELECT_1 = "Select1";
	const std::string AREA_SELECT_2 = "Select2";
	const std::string AREA_SELECT_3 = "Select3";
	const std::string AREA_SELECT_4 = "Select4";
	const std::string AREA_GO_BACK = "GoBack";

	const std::string IMAGE_SELECT_1 = "Selection1";
	const std::string IMAGE_SELECT_2 = "Selection2";
	const std::string IMAGE_SELECT_3 = "Selection3";
	const std::string IMAGE_SELECT_4 = "Selection4";
	const std::string IMAGE_HOVER_1 = "Hover1";
	const std::string IMAGE_HOVER_2 = "Hover2";
	const std::string IMAGE_HOVER_3 = "Hover3";
	const std::string IMAGE_HOVER_4 = "Hover4";

	const std::string WORLD_MAP_ID = "WorldMap";
	const std::string TEXT_HOVER_ISLAND = "HoverIsland";
	const std::string TEXT_GO_BACK = "GoBack";

	const std::string NO_HOVER_TEXT = "-";

	static std::optional<game::avatar::Destination> hoverDestinationId = std::nullopt;
	static game::avatar::Destination currentDestinationId = game::avatar::Destination::ONE;

	static void ReturnToGame()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<std::string, game::avatar::Destination> selectionTable =
	{
		{IMAGE_SELECT_1, game::avatar::Destination::ONE},
		{IMAGE_SELECT_2, game::avatar::Destination::TWO},
		{IMAGE_SELECT_3, game::avatar::Destination::THREE},
		{IMAGE_SELECT_4, game::avatar::Destination::FOUR}
	};

	static void RefreshSelection()
	{
		for (auto& entry : selectionTable)
		{
			visuals::Images::SetVisible(LAYOUT_NAME, entry.first, currentDestinationId == entry.second);
		}
	}

	const std::map<std::string, game::avatar::Destination> hoverTable =
	{
		{IMAGE_HOVER_1, game::avatar::Destination::ONE},
		{IMAGE_HOVER_2, game::avatar::Destination::TWO},
		{IMAGE_HOVER_3, game::avatar::Destination::THREE},
		{IMAGE_HOVER_4, game::avatar::Destination::FOUR}
	};

	static void RefreshHovers()
	{
		for (auto& entry : hoverTable)
		{
			visuals::Images::SetVisible(LAYOUT_NAME, entry.first, hoverDestinationId.has_value() && hoverDestinationId.value()==entry.second);
		}
	}

	static void Refresh()
	{
		RefreshHovers();
		RefreshSelection();
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, ReturnToGame },
		{ ::Command::RED, ReturnToGame }
	};

	static void HandleWorldMapMouseMotion(const common::XY<int>& location)
	{
		visuals::WorldMap::SetDestination(LAYOUT_NAME, WORLD_MAP_ID, location);
	}

	static void HandleMouseMotionInWorldMap(const common::XY<int>& location)
	{
		hoverDestinationId = std::nullopt;
		RefreshHovers();
		HandleWorldMapMouseMotion(location);
	}

	static std::function<void(const common::XY<int>&)> HoverOnDestinationId(const game::avatar::Destination& destinationId)
	{
		return [destinationId](const common::XY<int>&)
		{
			hoverDestinationId = destinationId;
			RefreshHovers();
		};
	}

	static void HiliteGoBackButton(bool hilite)
	{
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_GO_BACK, (hilite) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL));
	}

	void HoverOnGoBack(const common::XY<int>&)
	{
		HiliteGoBackButton(true);
	}

	const std::map<std::string, std::function<void(const common::XY<int>&)>> areaMotionHandlerTable =
	{
		{AREA_WORLD_MAP, HandleMouseMotionInWorldMap},
		{AREA_SELECT_1, HoverOnDestinationId(game::avatar::Destination::ONE)},
		{AREA_SELECT_2, HoverOnDestinationId(game::avatar::Destination::TWO)},
		{AREA_SELECT_3, HoverOnDestinationId(game::avatar::Destination::THREE)},
		{AREA_SELECT_4, HoverOnDestinationId(game::avatar::Destination::FOUR)},
		{AREA_GO_BACK, HoverOnGoBack}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		HiliteGoBackButton(false);
		auto entry = areaMotionHandlerTable.find(areaName);
		if (entry != areaMotionHandlerTable.end())
		{
			entry->second(location);
		}
	}
	
	static void OnMouseMotionOutsideArea(const common::XY<int>& location)
	{
		HiliteGoBackButton(false);
		hoverDestinationId = std::nullopt;
		RefreshHovers();
		visuals::WorldMap::SetDestination(LAYOUT_NAME, WORLD_MAP_ID, std::nullopt);
	}

	static bool HandleWorldMapMouseButtonUp()
	{
		game::avatar::destination::SetDestination(currentDestinationId,visuals::WorldMap::GetDestination(LAYOUT_NAME, WORLD_MAP_ID));
		return true;
	}

	static std::function<void()> SelectDestinationId(const game::avatar::Destination& destinationId)
	{
		return [destinationId]()
		{
			currentDestinationId = destinationId;
			RefreshSelection();
		};
	}

	const std::map<std::string, std::function<void()>> areaButtonHandlerTable =
	{
		{AREA_WORLD_MAP, HandleWorldMapMouseButtonUp},
		{AREA_SELECT_1, SelectDestinationId(game::avatar::Destination::ONE)},
		{AREA_SELECT_2, SelectDestinationId(game::avatar::Destination::TWO)},
		{AREA_SELECT_3, SelectDestinationId(game::avatar::Destination::THREE)},
		{AREA_SELECT_4, SelectDestinationId(game::avatar::Destination::FOUR)},
		{AREA_GO_BACK, application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA)}
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		auto entry = areaButtonHandlerTable.find(areaName);
		if (entry != areaButtonHandlerTable.end())
		{
			entry->second();
			return true;
		}
		return false;
	}

	static void OnUpdate(const unsigned int&)
	{
		auto hoverIsland = visuals::WorldMap::GetHoverIsland(LAYOUT_NAME, WORLD_MAP_ID);
		std::string hoverText = NO_HOVER_TEXT;
		if (hoverIsland)
		{
			auto islandModel = game::Islands::Read(hoverIsland.value()).value();
			hoverText =
				(islandModel.visits.has_value()) ? 
				(islandModel.name) :
				(game::Islands::UNKNOWN);
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_HOVER_ISLAND, hoverText);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		Refresh();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_HEAD_FOR, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_HEAD_FOR, visuals::Areas::HandleMouseMotion(LAYOUT_NAME,OnMouseMotionInArea,OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_HEAD_FOR, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_HEAD_FOR, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_HEAD_FOR, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_HEAD_FOR, OnUpdate);
	}
}
