#include <Game.Character.Actions.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.Temple.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_TEMPLE;
	static const std::string FORMAT_TEMPLE = "Temple of {}";

	static void Refresh()
	{
		auto islandId = GetPlayerCharacterIslandId().value();
		auto island = game::Islands::Read(islandId).value();
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(FORMAT_TEMPLE, game::Demigods::ReadName(island.patronDemigodId));
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Make offering");
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();

	}

	static void OnMakeOffering()
	{
		::application::UIState::Write(::UIState::IN_PLAY_MAKE_OFFERING);
	}

	static void OnLeave()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnMakeOffering },
		{ "0", OnLeave }
	};

	void Temple::Start()
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