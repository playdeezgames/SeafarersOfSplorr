#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Quest.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Islands.Quests.h>
#include "Game.Ship.h"
#include "State.InPlay.CurrentJob.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CURRENT_JOB;
	static const std::string LAYOUT_NAME = "State.Terminal";

	//static const std::map<CurrentJobMenuItem, std::function<void()>> activators =
	//{
	//	{ CurrentJobMenuItem::ABANDON, ::application::UIState::GoTo(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB) },
	//	{ CurrentJobMenuItem::CANCEL, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) }
	//};

	static void RefreshQuest(const game::Quest& questModel)
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Current Job:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto islandModel = game::Islands::Read(questModel.destination).value();
		double distance = common::Heading::Distance(questModel.destination, game::Ship::GetLocation());
		Terminal::WriteLine(
			"Please deliver this {} to {} the {} at {} ({:.2f}). Reward: {:.2f}",
			questModel.itemName,
			questModel.personName,
			questModel.professionName,
			islandModel.name, 
			distance,
			questModel.reward);
		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Abandon");
		Terminal::WriteLine("2) Never mind");

		Terminal::ShowPrompt();
	}

	static void RefreshNoQuest()
	{
		Terminal::ErrorMessage("You currently have no job.");
	}

	static void Refresh()
	{
		auto quest = game::avatar::Quest::Read();
		if (quest)
		{
			RefreshQuest(quest.value());
		}
		else
		{
			RefreshNoQuest();
			application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "2", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW)},
	};


	void CurrentJob::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE,
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE,
			LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 2.",
				Refresh));
	}
}