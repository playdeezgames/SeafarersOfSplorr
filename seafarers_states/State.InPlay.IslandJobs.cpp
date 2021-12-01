#include <Common.Heading.h>
#include <Data.Game.Island.h>
#include <Game.Character.Quest.h>
#include <Game.Islands.h>
#include <Game.Islands.Quests.h>
#include <Game.Ship.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandJobs.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_JOBS;

	static void OnAccept()//TODO: make this more declarative
	{
		switch (game::character::Quest::Accept(GetPlayerCharacterId(), data::game::Island::Find(
			data::game::Island::Read(GetPlayerCharacterIslandId().value()).value().location
		).value()))
		{
		case game::character::AcceptQuestResult::ACCEPTED_QUEST:
			DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			break;
		case game::character::AcceptQuestResult::ALREADY_HAS_QUEST:
			::application::UIState::Write(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB);
			break;
		}
	}

	static void OnCancel()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void RefreshQuest(const game::Quest& questModel)
	{
		auto islandModel = game::Islands::Read(questModel.toIslandId).value();
		double distance = common::Heading::Distance(questModel.destination, GetPlayerCharacterShipLocation().value());

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
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void RefreshNoQuest()
	{
		Terminal::ErrorMessage("There are currently no jobs available. Try again later.");
	}

	static void Refresh()
	{
		auto islandId =
			GetPlayerCharacterIslandId().value();
		auto quest = game::islands::Quests::Read(islandId);
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
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnAccept},
		{"0", OnCancel}
	};

	void IslandJobs::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}