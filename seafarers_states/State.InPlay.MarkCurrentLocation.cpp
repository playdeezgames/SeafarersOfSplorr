#include "State.InPlay.MarkCurrentLocation.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include <Application.TextInput.h>
#include <Game.Session.Player.h>
namespace state::in_play
{
	std::optional<int> MarkCurrentLocation::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
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
			auto character = game::session::Player().GetCharacter();
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
		Registrar::Register(stateId, [](int stateId)
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					HandleKey);
				::application::TextInput::AddHandler(
					stateId,
					HandleText);
			});
	}
}