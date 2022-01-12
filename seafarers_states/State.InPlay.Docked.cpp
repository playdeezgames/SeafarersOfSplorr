#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Docked.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.IslandDistrict.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCKED;
	static const std::string FORMAT_NAME = "Island Name: {}";

	static void Refresh()
	{
		auto island = game::Session().GetPlayer().GetCharacter().GetIsland();
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Docked:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(FORMAT_NAME, island.GetName());
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void OnUndock()
	{
		game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().Undock();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static std::function<void()> DoGoToDistrict(const game::session::island::District& district)
	{
		return [district]() 
		{
			scratch_pad::IslandDistrict::SetDistrict(district.operator game::island::District());
			application::UIState::Write(::UIState::IN_PLAY_ISLAND_DISTRICT);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto districts = 
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetIsland()
			.GetDistricts()
			.GetDistricts();
		std::for_each(
			districts.begin(),
			districts.end(),
			[](const game::session::island::District& district) 
			{
				Terminal::menu.AddAction({ district.GetName(), DoGoToDistrict(district) });
			});
		MenuAction defaultAction = { "Undock", OnUndock };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void Docked::Start()
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