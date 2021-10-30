#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Log.h>
#include <Game.Avatar.Quest.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Islands.Quests.h>
#include "Game.Ship.h"
#include "State.InPlay.IslandJobs.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_JOBS;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void OnAccept()//TODO: make this more declarative
	{
		switch (game::avatar::Quest::Accept(game::avatar::Docked::ReadLocation().value()))
		{
		case game::avatar::AcceptQuestResult::ACCEPTED_QUEST:
			game::avatar::Actions::DoAction(game::avatar::Action::ENTER_DOCK);
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			break;
		case game::avatar::AcceptQuestResult::ALREADY_HAS_QUEST:
			::application::UIState::Write(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB);
			break;
		}
	}

	static void OnCancel()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void RefreshQuest(const game::Quest& questModel)
	{
		auto islandModel = game::Islands::Read(questModel.destination).value();
		double distance = common::Heading::Distance(questModel.destination, game::Ship::GetLocation());

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Job Board:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(
			"Please deliver this {} to {} the {} at {} ({:.2f}). Reward: {:.2f}",
			questModel.itemName,
			questModel.personName,
			questModel.professionName,
			islandModel.name,
			distance,
			questModel.reward);

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Accept");
		Terminal::WriteLine("2) Never mind");

		Terminal::ShowPrompt();
	}

	static void RefreshNoQuest()
	{
		Terminal::ErrorMessage("There are currently no jobs available. Try again later.");
	}

	static void Refresh()
	{
		auto location = game::avatar::Docked::ReadLocation().value();
		auto quest = game::islands::Quests::Read(location);
		if (quest)
		{
			RefreshQuest(quest.value());
		}
		else
		{
			RefreshNoQuest();
			OnCancel();
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnAccept},
		{"2", OnCancel}
	};

	void IslandJobs::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 2.",
				Refresh));
	}
}