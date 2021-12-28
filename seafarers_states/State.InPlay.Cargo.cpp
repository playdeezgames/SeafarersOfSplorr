#include <Common.Data.h>
#include <Game.Characters.Ships.h>
#include <Game.Items.h>
#include <Game.Session.h>
#include "State.InPlay.Cargo.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CARGO;

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT);

	static void Refresh()
	{
		auto playerCharacter = game::Session().GetPlayer().GetCharacter();
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Cargo:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto availableTonnage = playerCharacter.GetAvailableTonnage();
		Terminal::SetForeground(game::Colors::BROWN);
		Terminal::WriteLine("Manifest:");
		Terminal::SetForeground(game::Colors::YELLOW);
		size_t index = 1;
		Terminal::WriteLine("0) Never mind");
		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	static void OnHandleOther(const std::string& input)
	{
		auto index = common::Data::ToInt(input)-1;
		Terminal::ErrorMessage("Please select a valid option.");
		Refresh();
	}

	void Cargo::Start()
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
				OnHandleOther));
	}
}