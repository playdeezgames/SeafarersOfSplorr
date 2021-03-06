#include <Game.Session.h>
#include <Game.Session.Characters.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewDetail.Characteristics.h"
#include "State.InPlay.CrewDetail.Deliveries.h"
#include "State.InPlay.CrewDetail.Inventory.h"
#include "State.InPlay.CrewList.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.CrewDetail.h"
#include <Game.Session.Character.HitPoints.h>
#include <Game.Session.Character.Flags.h>
#include <Game.Session.Tribe.h>
#include <Game.Session.Character.ItemTypes.h>
#include <Game.Session.Character.Deliveries.h>
#include <Game.Session.Island.h>
namespace state::in_play
{
	std::optional<int> CrewDetail::stateId = std::nullopt;

	static auto OnLeave = application::UIState::DoGoTo(CrewList::GetStateId);

	static void RefreshFlags(int characterId)
	{
		auto flags = game::session::character::Flags(characterId).GetAll();
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
			game::session::character::HitPoints(characterId);
		Terminal::WriteLine(
			"HP: {}/{}", 
			hitPoints.GetCurrent(),
			hitPoints.GetMaximum());
	}

	static void Refresh()
	{
		auto character = 
			game::session::Character(scratch_pad::CrewDetail::GetCharacterId());

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", character.GetName());
		Terminal::WriteLine("From: {}", game::session::Island(character.GetOriginIslandId()).GetName());
		Terminal::WriteLine("Tribe: {}", game::session::Tribe(character.GetTribeId()).GetName());
		RefreshFlags(character.ToId());
		RefreshHitPoints(character.ToId());

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
		if (game::session::character::ItemTypes(characterId).HasAny())
		{
			Terminal::menu.AddAction({"Inventory", application::UIState::DoGoTo(crew_detail::Inventory::GetStateId) });
		}
		if (game::session::character::Deliveries(character.ToId()).HasAny())
		{
			Terminal::menu.AddAction({ "Deliveries", application::UIState::DoGoTo(crew_detail::Deliveries::GetStateId) });
		}
		//TODO: plights
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