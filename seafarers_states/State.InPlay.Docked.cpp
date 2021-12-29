#include <Game.Islands.Features.h>
#include <Game.Session.h>
#include <Game.Ship.Docked.h>
#include "State.InPlay.Docked.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCKED;
	static const std::string FORMAT_NAME = "Island Name: {}";
	static const std::string FORMAT_VISITS = "Visits: {}";
	static const std::string FORMAT_TEMPLE = "5) Temple of {}";

	static void Refresh()
	{
		auto characterId = GetPlayerCharacterId();
		auto islandId = GetPlayerCharacterIslandId().value();
		auto island = game::Session().GetWorld().GetIslands().GetIsland(islandId);
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Docked:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(FORMAT_NAME, island.GetName());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Undock");

		Terminal::ShowPrompt();
	}

	static void OnUndock()
	{
		game::ship::Docked::Undock(GetPlayerCharacterShipId().value());
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnJob()
	{
		DoPlayerCharacterAction(game::characters::Action::ENTER_JOB_BOARD);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnTrade()
	{
		DoPlayerCharacterAction(game::characters::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<game::Feature, game::characters::Action> featureActionMap =
	{
		{game::Feature::SHIPYARD, game::characters::Action::ENTER_SHIPYARD},
		{game::Feature::DARK_ALLEY, game::characters::Action::ENTER_DARK_ALLEY},
		{game::Feature::TAVERN, game::characters::Action::ENTER_TAVERN},
	};

	static std::function<void()> OnEnterFeature(const game::Feature& feature)
	{
		return [feature]() 
		{
			if (PlayerCharacterIslandHasFeature(feature).value())
			{
				DoPlayerCharacterAction(featureActionMap.find(feature)->second);
				::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
			else
			{
				Terminal::ErrorMessage("Please select a valid option.");
				Refresh();
			}
		};
	}

	static void OnTemple()
	{
		DoPlayerCharacterAction(game::characters::Action::ENTER_TEMPLE);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", OnUndock}
	};

	void Docked::Start()
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