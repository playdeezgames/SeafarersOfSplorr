#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.LoadGame.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::LOAD_GAME;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Load Game:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		for (int slot = 1; slot <= 5; ++slot)
		{
			if (game::DoesSlotExist(slot))
			{
				Terminal::WriteLine("{}) Slot {}", slot, slot);
			}
		}
		if (game::DoesAutosaveExist())
		{
			Terminal::WriteLine("6) Autosave Slot");
		}
		Terminal::WriteLine("7) Never mind");

		Terminal::ShowPrompt();

	}

	static void GoBack()
	{
		::application::UIState::Write(::UIState::START_GAME);
	}

	static void LoadFromAutosave()
	{
		if (game::DoesAutosaveExist())
		{
			game::LoadFromAutosave();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage("Please select a valid option.");
		}
	}

	static std::function<void()> SlotLoader(int slot)
	{
		return [slot]()
		{
			if (game::DoesSlotExist(slot))
			{
				game::LoadFromSlot(slot);
				application::UIState::Write(::UIState::TIP);
			}
			else
			{
				Terminal::ErrorMessage("Please select a valid option.");
			}
		};
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", SlotLoader(1)},
		{"2", SlotLoader(2)},
		{"3", SlotLoader(3)},
		{"4", SlotLoader(4)},
		{"5", SlotLoader(5)},
		{"6", LoadFromAutosave},
		{"7", GoBack}
	};

	void LoadGame::Start()
	{
		::application::Renderer::SetRenderLayout(::UIState::LOAD_GAME, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::LOAD_GAME, OnEnter);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 5.",
				Refresh));
	}
}