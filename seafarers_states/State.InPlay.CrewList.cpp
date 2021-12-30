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
		int index;
		std::string name;
		std::string berth;
		std::string mark;
		int characterId;
		int currentHitPoints;
		int maximumHitPoints;
	};

	static std::vector<RosterItem> rosterItems;

	static void ShowRosterItem(const RosterItem& rosterItem)
	{
		Terminal::WriteLine(
			"{}) {}{} - {} HP:{}/{}", 
			rosterItem.index, 
			rosterItem.name, 
			rosterItem.mark, 
			rosterItem.berth,
			rosterItem.currentHitPoints,
			rosterItem.maximumHitPoints);
	}

	static void RefreshRoster()
	{
		std::for_each(
			rosterItems.begin(), 
			rosterItems.end(), 
			ShowRosterItem);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew List:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshRoster();
		Terminal::WriteLine("0) Never mind");

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

	static void UpdateRoster()
	{
		rosterItems.clear();
		auto crew = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().GetBerths().GetBerths();
		int index = 1;
		std::transform(
			crew.begin(), 
			crew.end(), 
			std::back_inserter(rosterItems), 
			[&index](const game::session::ship::Berth& entry) 
			{
				auto characterId = entry.GetCharacterId();
				auto character = game::Session().GetCharacters().GetCharacter(characterId);
				auto hitPoints = 
					game::Session()
					.GetCharacters()
					.GetCharacter(characterId)
					.GetHitpoints();
				RosterItem result = {
					index++,
					character.GetName(),
					berthNames.find(entry.GetBerthType())->second,
					(characterId==GetPlayerCharacterId()) ? ("(you)") : (""),
					characterId,
					hitPoints.GetCurrent(),
					hitPoints.GetMaximum()
					};
				return result;
			});
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateRoster();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave}
	};

	static const void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < rosterItems.size())
		{
			SetCrewDetailCharacterId(rosterItems[index].characterId);
			application::UIState::Write(::UIState::IN_PLAY_CREW_DETAIL);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
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
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}