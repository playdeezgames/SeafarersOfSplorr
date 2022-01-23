#include "State.InPlay.CrewList.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.CrewDetail.h"
#include <algorithm>
#include <Game.Session.Characters.h>
#include <Game.Session.Player.h>
#include <Game.Session.Character.HitPoints.h>
#include <Game.BerthType.h>
#include <Game.Session.Character.Berth.h>
namespace state::in_play
{
	std::optional<int> CrewList::stateId = std::nullopt;

	static auto OnLeave = ::application::UIState::DoGoTo(AtSeaOverview::GetStateId);

	struct RosterItem
	{
		std::string text;
		int characterId;
	};

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew List:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static const std::map<game::BerthType, std::string> berthNames =
	{
		{game::BerthType::CAPTAIN, "Captain"},
		{game::BerthType::CAPTIVE, "Captive"},
		{game::BerthType::CREW, "Crew"},
		{game::BerthType::OFFICER, "Officer"},
		{game::BerthType::PASSENGER, "Passenger"}
	};

	static const std::function<void()> GoToCrewDetail(int characterId)
	{
		return [characterId]()
		{
			scratch_pad::CrewDetail::SetCharacterId(characterId);
			application::UIState::Write(CrewDetail::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		std::list<RosterItem> rosterItems;
		auto crew = 
			game::session::character::Berth(game::session::Player::GetCharacterId())
			.GetShip()
			.GetBerths()
			.GetBerths();
		std::transform(
			crew.begin(),
			crew.end(),
			std::back_inserter(rosterItems),
			[](const game::session::ship::Berth& entry)
			{
				auto characterId = entry.GetCharacterId();
				auto character = game::session::Characters().GetCharacter(characterId);
				auto hitPoints =
					game::session::character::HitPoints(characterId);
				RosterItem result = {
					std::format(
						"{}{} - {} HP{}/{}", 
						character.GetName(), 
						(character.IsPlayer()) ? ("(you)") : (""),					
						berthNames.find(entry.GetBerthType())->second, 
						hitPoints.GetCurrent(),					
						hitPoints.GetMaximum()),
					characterId
				};
				return result;
			});
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		std::for_each(
			rosterItems.begin(), 
			rosterItems.end(), 
			[](const RosterItem& rosterItem) 
			{
				Terminal::menu.AddAction({ rosterItem.text, GoToCrewDetail(rosterItem.characterId) });
			});
		MenuAction defaultAction = { "Never mind", OnLeave };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void CrewList::Start()
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