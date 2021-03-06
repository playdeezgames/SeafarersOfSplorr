#include "State.InPlay.MarkCurrentLocation.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include <Application.TextInput.h>
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Marks.h>
#include <Game.Session.Character.Berth.h>
#include <Game.Session.Ship.h>
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
			auto character = game::session::Character(game::session::Player::GetCharacterId());
			auto marks = game::session::character::Marks(character.ToId());
			auto location = 
				game::session::Ship(
				game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId())
				.GetLocation();
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
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					HandleKey);
				::application::TextInput::AddHandler(
					currentState,
					HandleText);
			});
	}
}