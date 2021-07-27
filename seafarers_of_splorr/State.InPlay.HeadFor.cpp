#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Islands.h"
#include "Visuals.Areas.h"
#include "Visuals.Texts.h"
#include "Visuals.WorldMap.h"
namespace state::in_play::HeadFor
{
	const std::string LAYOUT_NAME = "State.InPlay.HeadFor";
	const std::string AREA_WORLD_MAP = "WorldMap";
	const std::string WORLD_MAP_ID = "WorldMap";
	const std::string TEXT_HOVER_ISLAND = "HoverIsland";

	static void ReturnToGame()
	{
		::application::UIState::EnterGame();
	}

	static void ActivateItem()
	{

	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		//{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		//{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, ReturnToGame },
		{ ::Command::RED, ReturnToGame },
		{ ::Command::GREEN, ActivateItem }
	};

	static void HandleWorldMapMouseMotion(const common::XY<int>& location)
	{
		visuals::WorldMap::SetDestination(LAYOUT_NAME, WORLD_MAP_ID, location);
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		if (areaName == AREA_WORLD_MAP)
		{
			HandleWorldMapMouseMotion(location);
			return;
		}
	}
	
	static void OnMouseMotionOutsideArea(const common::XY<int>& location)
	{
		visuals::WorldMap::SetDestination(LAYOUT_NAME, WORLD_MAP_ID, std::nullopt);
	}

	static bool HandleWorldMapMouseButtonUp()
	{
		game::Avatar::SetDestination(visuals::WorldMap::GetDestination(LAYOUT_NAME, WORLD_MAP_ID));
		return true;
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (areaName == AREA_WORLD_MAP)
		{
			return HandleWorldMapMouseButtonUp();
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

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_HEAD_FOR, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_HEAD_FOR, visuals::Areas::HandleMouseMotion(LAYOUT_NAME,OnMouseMotionInArea,OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_HEAD_FOR, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_HEAD_FOR, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_HEAD_FOR, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_HEAD_FOR, OnUpdate);
	}
}
