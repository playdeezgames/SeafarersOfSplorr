#include <algorithm>
#include <Common.Data.h>
#include <Game.BerthType.h>
#include <Game.Session.h>
#include <iterator>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewList.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_LIST;

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW);

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
			SetCrewDetailCharacterId(characterId);
			application::UIState::Write(::UIState::IN_PLAY_CREW_DETAIL);
		};
	}

	static void UpdateMenu()
	{
		std::list<RosterItem> rosterItems;
		auto crew = 
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetBerth()
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
				auto character = game::Session().GetCharacters().GetCharacter(characterId);
				auto hitPoints =
					game::Session()
					.GetCharacters()
					.GetCharacter(characterId)
					.GetHitpoints();
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
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}