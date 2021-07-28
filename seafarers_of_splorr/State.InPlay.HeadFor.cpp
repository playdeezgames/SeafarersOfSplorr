#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Destination.h"
#include "Game.Islands.h"
#include "Visuals.Areas.h"
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

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		if (areaName == AREA_WORLD_MAP)
		{
			hoverDestinationId = std::nullopt;
			RefreshHovers();
			HandleWorldMapMouseMotion(location);
			return;
		}
		if (areaName == AREA_SELECT_1)
		{
			hoverDestinationId = game::avatar::Destination::ONE;
			RefreshHovers();
			return;
		}
		if (areaName == AREA_SELECT_2)
		{
			hoverDestinationId = game::avatar::Destination::TWO;
			RefreshHovers();
			return;
		}
		if (areaName == AREA_SELECT_3)
		{
			hoverDestinationId = game::avatar::Destination::THREE;
			RefreshHovers();
			return;
		}
		if (areaName == AREA_SELECT_4)
		{
			hoverDestinationId = game::avatar::Destination::FOUR;
			RefreshHovers();
			return;
		}
	}
	
	static void OnMouseMotionOutsideArea(const common::XY<int>& location)
	{
		hoverDestinationId = std::nullopt;
		RefreshHovers();
		visuals::WorldMap::SetDestination(LAYOUT_NAME, WORLD_MAP_ID, std::nullopt);
	}

	static bool HandleWorldMapMouseButtonUp()
	{
		game::avatar::destination::SetDestination(currentDestinationId,visuals::WorldMap::GetDestination(LAYOUT_NAME, WORLD_MAP_ID));
		return true;
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (areaName == AREA_WORLD_MAP)
		{
			return HandleWorldMapMouseButtonUp();
		}
		if (areaName == AREA_SELECT_1)
		{
			currentDestinationId = game::avatar::Destination::ONE;
			RefreshSelection();
			return true;
		}
		if (areaName == AREA_SELECT_2)
		{
			currentDestinationId = game::avatar::Destination::TWO;
			RefreshSelection();
			return true;
		}
		if (areaName == AREA_SELECT_3)
		{
			currentDestinationId = game::avatar::Destination::THREE;
			RefreshSelection();
			return true;
		}
		if (areaName == AREA_SELECT_4)
		{
			currentDestinationId = game::avatar::Destination::FOUR;
			RefreshSelection();
			return true;
		}
		return false;
	}

	static void OnUpdate(const unsigned int&)//TODO: refactor me
	{
		auto hoverIsland = visuals::WorldMap::GetHoverIsland(LAYOUT_NAME, WORLD_MAP_ID);
		if (hoverIsland)
		{
			auto islandModel = game::Islands::Read(hoverIsland.value()).value();
			if(islandModel.visits)
			{ 
				visuals::Texts::SetText(LAYOUT_NAME, TEXT_HOVER_ISLAND, islandModel.name);
			}
			else
			{
				visuals::Texts::SetText(LAYOUT_NAME, TEXT_HOVER_ISLAND, "????");//TODO: hard coded
			}
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_HOVER_ISLAND, "-");
		}
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
