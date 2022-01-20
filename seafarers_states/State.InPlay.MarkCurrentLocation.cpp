#include <Application.TextInput.h>
#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.MarkCurrentLocation.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MARK_LOCATION;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			game::Session()
			.GetPlayer().GetCharacter();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Location name?");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Terminal::ClearInput();
		Refresh();
	}

	static bool HandleKey(const std::string& key)
	{
		const std::string BACKSPACE = "Backspace";
		const std::string RETURN = "Return";
		if (key == BACKSPACE)
		{
			Terminal::HandleBackspace();
			return true;
		}
		if (key == RETURN)
		{
			auto locationName = Terminal::GetInput();
			auto character = game::Session().GetPlayer().GetCharacter();
			auto marks = character.GetMarks();
			auto location = character.GetBerth().GetShip().GetLocation();
			marks.AddMark(locationName, location);
			Terminal::WriteLine();
			application::UIState::Write(ShipStatus::GetStateId());
			return true;
		}
		return false;
	}

	static void HandleText(const std::string& text)
	{
		Terminal::AppendInput(text);
		Terminal::Write(text);
	}

	void MarkCurrentLocation::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			HandleKey);
		::application::TextInput::AddHandler(
			CURRENT_STATE, 
			HandleText);
	}
}