#include "State.InPlay.Globals.h"
#include <Game.Session.h>
#include "State.InPlay.IslandDistrict.h"
#include "State.ScratchPad.IslandDistrict.h"
namespace state::in_play
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_DISTRICT;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto district =
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetIsland()
			.GetDistricts()
			.GetDistrict(scratch_pad::IslandDistrict::GetDistrict());
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(district.GetName());
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		//Terminal::menu.AddAction({ "Dock", OnDock });
		MenuAction defaultAction = { "Leave district", application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void IslandDistrict::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}