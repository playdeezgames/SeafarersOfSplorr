#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Plights.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Buttons.h>
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AVATAR_STATUS;
	static const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string AREA_EQUIPMENT = "Equipment";
	static const std::string AREA_JOB = "Job";
	static const std::string AREA_GO_BACK = "GoBack";
	static const std::string BUTTON_EQUIPMENT = "Equipment";
	static const std::string BUTTON_JOB = "Job";
	static const std::string BUTTON_GO_BACK = "GoBack";
	static const std::string TEXT_TOOL_TIP = "ToolTip";

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);
	static const auto WriteTextToGridRight = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::RIGHT);

	static void OnJob()
	{
		if (visuals::Buttons::IsEnabled(LAYOUT_NAME, BUTTON_JOB))
		{
			::application::UIState::Write(::UIState::IN_PLAY_CURRENT_JOB);
		}
	}

	static const auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT);

	static void UpdateStatistics()
	{
		WriteTextToGridRight({ 39,0 }, std::format(game::avatar::Statistics::FORMAT_MONEY, game::avatar::Statistics::GetMoney()), game::Colors::YELLOW);
		WriteTextToGridRight({ 39,1 }, std::format(game::avatar::Statistics::FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()), game::Colors::BLUE);
		WriteTextToGridRight({ 39,2 }, std::format(game::avatar::Statistics::FORMAT_BRAWLING, game::avatar::Statistics::GetBrawling()), game::Colors::BROWN);
		WriteTextToGridRight({ 39,3 }, std::format(game::avatar::Statistics::FORMAT_HEALTH, game::avatar::Statistics::GetHealth()), game::Colors::RED);
		WriteTextToGridRight({ 39,4 }, std::format(game::avatar::Statistics::FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()), game::Colors::MAGENTA);
		WriteTextToGridRight({ 39,5 }, std::format(game::avatar::Statistics::FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()), game::Colors::DARK_GRAY);
	}

	static void UpdatePlights()
	{
		WriteTextToGrid({ 0,0 }, "Plights:", game::Colors::GRAY);
		auto inflicted = game::avatar::Plights::InflictedWith();
		int row = 1;
		if (inflicted.empty())
		{
			WriteTextToGrid({ 0, row }, "(none)", game::Colors::GRAY);
			return;
		}
		for (auto& plight : inflicted)
		{
			auto descriptor = game::avatar::Plights::Read(plight);
			WriteTextToGrid({ 0,row++ }, descriptor.name, (descriptor.type == game::avatar::PlightType::CURSE) ? (game::Colors::RED) : (game::Colors::GREEN));
		}
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		UpdateStatistics();
		UpdatePlights();
	}

	static void RefreshMenu()
	{
		visuals::Buttons::SetEnabled(LAYOUT_NAME, BUTTON_JOB, game::avatar::Quest::Read().has_value());
	}

	static void Refresh()
	{
		RefreshGrid();
		RefreshMenu();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	static const std::map<std::string, std::string> areaButtons = 
	{
		{AREA_GO_BACK, BUTTON_GO_BACK},
		{AREA_EQUIPMENT, BUTTON_EQUIPMENT},
		{AREA_JOB, BUTTON_JOB}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>&)
	{
		visuals::Buttons::ClearHoverButton(LAYOUT_NAME);
		auto iter = areaButtons.find(areaName);
		if (iter != areaButtons.end())
		{
			visuals::Buttons::SetHoverButton(LAYOUT_NAME, iter->second);
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TOOL_TIP, visuals::Areas::Get(LAYOUT_NAME, areaName).value().toolTip.value_or(""));
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Buttons::ClearHoverButton(LAYOUT_NAME);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TOOL_TIP, "");
	}

	static const std::map<std::string, std::function<void()>> areaActions =
	{
		{AREA_GO_BACK, OnLeave},
		{AREA_EQUIPMENT, ::application::UIState::GoTo(::UIState::IN_PLAY_EQUIPMENT)},
		{AREA_JOB, OnJob}
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		auto iter = areaActions.find(areaName);
		if (iter != areaActions.end())
		{
			iter->second();
			return true;
		}
		return false;
	}

	void AvatarStatus::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
