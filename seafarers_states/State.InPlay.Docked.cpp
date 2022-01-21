#include "State.InPlay.Docked.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.InPlay.Next.h"
#include "State.ScratchPad.IslandDistrict.h"
#include <algorithm>
#include <Game.Session.Player.h>
namespace state::in_play
{
	std::optional<int> Docked::stateId = std::nullopt;
	static const std::string FORMAT_NAME = "Island Name: {}";

	static void Refresh()
	{
		auto island = game::session::Player().GetCharacter().GetIsland();
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
		game::session::Player().GetCharacter().GetBerth().GetShip().Undock();
		application::UIState::Write(Next::GetStateId());
	}

	static std::function<void()> DoGoToDistrict(const game::session::island::District& district)
	{
		return [district]() 
		{
			scratch_pad::IslandDistrict::SetDistrict(district.operator game::island::District());
			application::UIState::Write(IslandDistrict::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto districts = 
			game::session::Player()
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
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}