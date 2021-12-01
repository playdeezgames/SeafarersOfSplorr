#include <Common.Data.h>
#include <Game.Ship.h>
#include <Game.ShipNames.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.RenameShipAdjective.h"
#include "State.InPlay.RenameShipNoun.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_RENAME_SHIP_NOUN;

	static std::map<int, std::string> nouns;

	static void UpdateNouns()
	{
		nouns.clear();
		int index = 1;
		for (auto noun : game::ShipNames::GetNouns())
		{
			nouns[index++] = noun;
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Select Noun:");

		Terminal::SetForeground(game::Colors::YELLOW);
		for (auto entry : nouns)
		{
			Terminal::WriteLine("{}) {}", entry.first, entry.second);
		}

		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateNouns();
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
			if (nouns.contains(index.value()))
			{
				auto noun = nouns[index.value()];
				auto shipId = GetPlayerCharacterShipId().value();
				game::Ship::SetName(shipId, std::format("{} {}", GetRenameShipAdjective(), noun));
				application::UIState::Write(::UIState::IN_PLAY_SHIP_STATUS);
				return;
			}
		}
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	void RenameShipNoun::Start()
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