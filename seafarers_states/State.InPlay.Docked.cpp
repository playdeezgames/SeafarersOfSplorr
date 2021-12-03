#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Features.h>
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
		auto islandId = GetPlayerCharacterIslandId().value();
		auto island = game::Islands::Read(islandId).value();
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Docked:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(FORMAT_NAME, island.name);
		Terminal::WriteLine(FORMAT_VISITS, island.visits.value_or(0));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Jobs");
		Terminal::WriteLine("2) Trade");
		if (PlayerCharacterIslandHasFeature(game::Feature::SHIPYARD))
		{
			Terminal::WriteLine("3) Shipyard");
		}
		if (PlayerCharacterIslandHasFeature(game::Feature::DARK_ALLEY))
		{
			Terminal::WriteLine("4) Dark Alley");
		}
		Terminal::WriteLine(FORMAT_TEMPLE, game::Demigods::ReadName(island.patronDemigodId));
		if (PlayerCharacterIslandHasFeature(game::Feature::TAVERN))
		{
			Terminal::WriteLine("6) Tavern");
		}
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
		DoPlayerCharacterAction(game::character::Action::ENTER_JOB_BOARD);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnTrade()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<game::Feature, game::character::Action> featureActionMap =
	{
		{game::Feature::SHIPYARD, game::character::Action::ENTER_SHIPYARD},
		{game::Feature::DARK_ALLEY, game::character::Action::ENTER_DARK_ALLEY},
		{game::Feature::TAVERN, game::character::Action::ENTER_TAVERN},
	};

	static std::function<void()> OnEnterFeature(const game::Feature& feature)
	{
		return [feature]() 
		{
			if (PlayerCharacterIslandHasFeature(feature))
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
		DoPlayerCharacterAction(game::character::Action::ENTER_TEMPLE);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnJob},
		{"2", OnTrade},
		{"3", OnEnterFeature(game::Feature::SHIPYARD)},
		{"4", OnEnterFeature(game::Feature::DARK_ALLEY)},
		{"5", OnTemple},
		{"6", OnEnterFeature(game::Feature::TAVERN)},
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
				"Please enter a number between 1 and 6",
				Refresh));
	}
}