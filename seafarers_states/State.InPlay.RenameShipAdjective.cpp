#include <Common.Data.h>
#include <Game.ShipNames.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.RenameShipAdjective.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_RENAME_SHIP_ADJECTIVE;

	static std::map<int, std::string> adjectives;
	static std::string adjective;

	const std::string& RenameShipAdjective::Read()
	{
		return adjective;
	}

	static void UpdateAdjectives()
	{
		adjectives.clear();
		int index = 1;
		for (auto adjective : game::ShipNames::GetAdjectives())
		{
			adjectives[index++] = adjective;
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Select Adjective:");

		Terminal::SetForeground(game::Colors::YELLOW);
		for (auto entry : adjectives)
		{
			Terminal::WriteLine("{}) {}", entry.first, entry.second);
		}

		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateAdjectives();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	static void OnOtherInput(const std::string& line)
	{
		auto index = common::Data::ToOptionalInt(line);
		if (index)
		{
			if (adjectives.contains(index.value()))
			{
				adjective = adjectives[index.value()];
				application::UIState::Write(::UIState::IN_PLAY_RENAME_SHIP_NOUN);
				return;
			}
		}
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	void RenameShipAdjective::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}