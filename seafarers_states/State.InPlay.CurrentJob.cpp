#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Character.Quest.h>
#include <Game.Character.Ship.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Islands.Quests.h>
#include <Game.Player.h>
#include "Game.Ship.h"
#include "State.InPlay.CurrentJob.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CURRENT_JOB;

	static void RefreshQuest(const game::Quest& questModel)
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Current Job:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto islandModel = game::Islands::Read(questModel.destination).value();
		auto delta = questModel.destination - game::Ship::GetLocation(game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value()).value();
		Terminal::WriteLine(
			"Please deliver this {} to {} the {} at {} ({:.2f}\xf8 distance {:.1f}). Reward: {:.2f}",
			questModel.itemName,
			questModel.personName,
			questModel.professionName,
			islandModel.name, 
			common::Heading::XYToDegrees(delta),
			delta.GetMagnitude(),
			questModel.reward);
		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Abandon");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void RefreshNoQuest()
	{
		Terminal::ErrorMessage("You currently have no job.");
	}

	static void Refresh()
	{
		auto quest = game::character::Quest::Read(game::Player::GetCharacterId());
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
		{ "1", application::UIState::GoTo(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB) },
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW)},
	};


	void CurrentJob::Start()
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
				Terminal::INVALID_INPUT,
				Refresh));
	}
}