#include <Game.Session.h>
#include <Game.Session.Characters.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewDetail.Characteristics.h"
#include "State.InPlay.CrewDetail.Deliveries.h"
#include "State.InPlay.CrewDetail.Inventory.h"
#include "State.InPlay.CrewList.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
#include "State.ScratchPad.CrewDetail.h"
namespace state::in_play
{
	std::optional<int> CrewDetail::stateId = std::nullopt;

	static auto OnLeave = application::UIState::DoGoTo(CrewList::GetStateId);

	static void RefreshFlags(int characterId)
	{
		auto character =
			game::session::Characters()
			.GetCharacter(characterId);
		auto flags = character.GetFlags().GetAll();
		if (!flags.empty())
		{
			bool first = true;
			for (auto flag : flags)
			{
				auto& name = flag.GetName();
				if (first)
				{
					Terminal::Write("Status: {}", name);
				}
				else
				{
					Terminal::Write(", {}", name);
				}
				first = false;
			}
			Terminal::WriteLine();
		}
	}

	static void RefreshHitPoints(int characterId)
	{
		auto hitPoints = 
			game::session::Characters()
			.GetCharacter(characterId)
			.GetHitpoints();
		Terminal::WriteLine(
			"HP: {}/{}", 
			hitPoints.GetCurrent(),
			hitPoints.GetMaximum());
	}

	static void Refresh()
	{
		int characterId = scratch_pad::CrewDetail::GetCharacterId();
		auto character = 
			game::session::Characters()
			.GetCharacter(characterId);

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", character.GetName());
		Terminal::WriteLine("From: {}", character.GetOriginIsland().GetName());
		Terminal::WriteLine("Tribe: {}", character.GetTribe().GetName());
		RefreshFlags(characterId);
		RefreshHitPoints(characterId);

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		int characterId = scratch_pad::CrewDetail::GetCharacterId();
		auto character =
			game::session::Characters()
			.GetCharacter(characterId);

		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Characteristics", application::UIState::DoGoTo(crew_detail::Characteristics::GetStateId) });
		if (character.GetItemTypes().HasAny())
		{
			Terminal::menu.AddAction({"Inventory", application::UIState::DoGoTo(crew_detail::Inventory::GetStateId) });
		}
		if (character.GetDeliveries().HasAny())
		{
			Terminal::menu.AddAction({ "Deliveries", application::UIState::DoGoTo(crew_detail::Deliveries::GetStateId) });
		}
		MenuAction defaultAction = { "Never mind", OnLeave };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void CrewDetail::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(
					currentState,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					currentState,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}