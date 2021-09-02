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
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const ::UIState CURRENT_LAYOUT = ::UIState::IN_PLAY_AVATAR_STATUS;
	static const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);
	static const auto WriteTextToGridRight = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::RIGHT);

	static void OnLeave()
	{
		::application::UIState::Pop();
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

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
		auto inflicted = game::avatar::Plights::GetInflicted();
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
