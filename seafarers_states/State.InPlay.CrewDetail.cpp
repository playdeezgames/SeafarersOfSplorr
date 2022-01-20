#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
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
			game::Session()
			.GetCharacters()
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
			game::Session()
			.GetCharacters()
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
			game::Session()
			.GetCharacters()
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
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId);

		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Characteristics", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_CHARACTERISTICS) });
		if (character.GetItemTypes().HasAny())
		{
			Terminal::menu.AddAction({"Inventory", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_INVENTORY) });
		}
		if (character.GetDeliveries().HasAny())
		{
			Terminal::menu.AddAction({ "Deliveries", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_DELIVERIES) });
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}