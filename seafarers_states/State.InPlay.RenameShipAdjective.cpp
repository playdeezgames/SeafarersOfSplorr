#include <algorithm>
#include <Common.Data.h>
#include <Game.Session.Player.h>
#include <Game.ShipNames.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.RenameShipAdjective.h"
#include "State.InPlay.RenameShipNoun.h"
#include "State.InPlay.ShipStatus.h"
#include "State.Registrar.h"
#include "State.ScratchPad.ShipAdjective.h"
namespace state::in_play
{
	std::optional<int> RenameShipAdjective::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Select Adjective:");

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoChooseAdjective(const std::string& adjective)
	{
		return [adjective]() 
		{
			scratch_pad::ShipAdjective::SetAdjective(adjective);
			application::UIState::Write(RenameShipNoun::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto adjectives = game::ShipNames::GetAdjectives();
		std::for_each(
			adjectives.begin(), 
			adjectives.end(), 
			[](const std::string& adjective) 
			{
				Terminal::menu.AddAction({ adjective, DoChooseAdjective(adjective) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ShipStatus::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void RenameShipAdjective::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}