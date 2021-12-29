#include <Common.Data.h>
#include <Game.BerthType.h>
#include <Game.Session.h>
#include <Game.Ship.Crew.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewList.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_LIST;

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW);

	struct RosterItem
	{
		std::string name;
		std::string berth;
		std::string mark;
		int characterId;
	};

	static std::vector<RosterItem> rosterItems;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew List:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		size_t index = 1;
		for (auto& rosterItem : rosterItems)
		{
			auto hitPoints = game::Session().GetCharacters().GetCharacter(rosterItem.characterId).GetHitpoints();
			Terminal::WriteLine(
				"{}) {}{} - {} HP:{}/{}", 
				index++, 
				rosterItem.name, 
				rosterItem.mark, 
				rosterItem.berth,
				hitPoints.GetCurrent(),
				hitPoints.GetMaximum());
		}
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
		auto crew = game::ship::Crew::ReadForCharacter(GetPlayerCharacterId());
		for (auto& entry : crew)
		{
			rosterItems.push_back({
				entry.name,
				berthNames.find(entry.berthType)->second,
				(entry.avatarId==GetPlayerCharacterId()) ? ("(you)") : (""),
				entry.avatarId
				});
		}
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