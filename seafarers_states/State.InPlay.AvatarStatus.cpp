#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const ::UIState CURRENT_LAYOUT = ::UIState::IN_PLAY_AVATAR_STATUS;
	static const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		::application::UIState::Pop();
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		WriteTextToGrid({ 0,0 }, std::format(game::avatar::Statistics::FORMAT_MONEY, game::avatar::Statistics::GetMoney()), game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format(game::avatar::Statistics::FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()), game::Colors::BLUE);
		WriteTextToGrid({ 0,2 }, std::format(game::avatar::Statistics::FORMAT_BRAWLING, game::avatar::Statistics::GetBrawling()), game::Colors::BROWN);
		WriteTextToGrid({ 0,3 }, std::format(game::avatar::Statistics::FORMAT_HEALTH, game::avatar::Statistics::GetHealth()), game::Colors::RED);
		WriteTextToGrid({ 0,4 }, std::format(game::avatar::Statistics::FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()), game::Colors::MAGENTA);
		WriteTextToGrid({ 0,5 }, std::format(game::avatar::Statistics::FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()), game::Colors::DARK_GRAY);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		RefreshGrid();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	void AvatarStatus::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_LAYOUT, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_LAYOUT, OnMouseButtonUp);
		::application::Command::SetHandlers(CURRENT_LAYOUT, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_LAYOUT, LAYOUT_NAME);
	}
}
