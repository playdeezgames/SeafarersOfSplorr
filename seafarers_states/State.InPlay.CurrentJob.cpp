#include <Common.Heading.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include "State.InPlay.CurrentJob.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CURRENT_JOB;

	static void RefreshQuest(int characterId, const game::Quest& questModel)
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Current Job:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto islandModel = game::Islands::Read(characterId, questModel.toIslandId).value();
		auto delta = questModel.destination - GetPlayerCharacterShipLocation().value();
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
		auto characterId = GetPlayerCharacterId();
		auto quest = GetPlayerCharacterQuest();
		if (quest)
		{
			RefreshQuest(characterId, quest.value());
			return;
		}
		RefreshNoQuest();
		application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
	}

	static void OnEnter()
	{
		PlayMainTheme();
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