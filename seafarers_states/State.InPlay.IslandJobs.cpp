#include <Common.Heading.h>
#include <Game.Session.h>
#include <Game.Ship.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandJobs.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_JOBS;

	static void OnCancel()
	{
		DoPlayerCharacterAction(game::characters::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void RefreshQuest(int characterId, const game::Quest& questModel)
	{
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
			game::Session().GetWorld().GetIslands().GetIsland(questModel.toIslandId).GetName(),
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
		auto characterId = GetPlayerCharacterId();
		auto islandId =
			GetPlayerCharacterIslandId().value();
		RefreshNoQuest();
		OnCancel();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
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