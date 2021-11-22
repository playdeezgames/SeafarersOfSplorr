#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Character.Items.h>
#include <Game.Character.Ship.h>
#include <Game.Avatar.Statistics.h>
#include <Game.BerthType.h>
#include <Game.Colors.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include <Game.Player.h>
#include <Game.Ship.Crew.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewList.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_LIST;

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW);

	struct RosterItem
	{
		std::string name;
		std::string berth;
		std::string mark;
		int avatarId;
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
			Terminal::WriteLine("{}) {}{} - {}", index++, rosterItem.name, rosterItem.mark, rosterItem.berth);
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
		auto crew = game::ship::Crew::Read();
		for (auto& entry : crew)
		{
			rosterItems.push_back({
				entry.name,
				berthNames.find(entry.berthType)->second,
				(entry.avatarId==game::Player::GetAvatarId()) ? ("(you)") : (""),
				entry.avatarId
				});
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
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
			CrewDetail::SetAvatarId(rosterItems[index].avatarId);
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