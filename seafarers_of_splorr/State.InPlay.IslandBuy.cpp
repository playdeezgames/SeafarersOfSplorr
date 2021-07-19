#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Visuals.SpriteGrid.h"
#include "Game.Items.h"
#include "Game.Islands.Items.h"
#include "Game.Avatar.h"
namespace state::in_play::IslandBuy
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandBuy";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";
	const std::string COLOR_DEFAULT = "Gray";

	static void WriteTextToGrid(const common::XY<int> location, const std::string& text, const std::string& color)
	{
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, location, FONT_DEFAULT, text, color);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		::application::UIState::Write(::UIState::IN_PLAY_ISLAND_TRADE);
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", COLOR_DEFAULT);
		WriteTextToGrid({ 37, 0 }, "->", COLOR_DEFAULT);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", COLOR_DEFAULT);
	}

	static std::map<game::Item, double> unitPrices;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetPrices(game::Avatar::GetDockedLocation().value());
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateUnitPrices();
		RefreshHeader();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_BUY, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_BUY, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_ISLAND_BUY, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_BUY, LAYOUT_NAME);
	}
}
