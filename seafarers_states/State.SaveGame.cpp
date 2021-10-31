#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.SaveGame.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::SAVE_GAME;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Save Game:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		for (int slot = 1; slot <= 5; ++slot)
		{
			if (game::DoesSlotExist(slot))
			{
				Terminal::WriteLine("{}) Slot {}(overwrite)", slot, slot);
			}
			else
			{
				Terminal::WriteLine("{}) Slot {}", slot, slot);
			}
		}
		if (game::DoesAutosaveExist())
		{
			Terminal::WriteLine("6) Autosave Slot(overwrite)");
		}
		else
		{
			Terminal::WriteLine("6) Autosave Slot");
		}
		Terminal::WriteLine("7) Never mind");

		Terminal::ShowPrompt();
	}

	static void SaveToAutosave()
	{
		game::AutoSave();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static std::function<void()> SlotSaver(int slot, ::UIState confirmState)
	{
		return [slot, confirmState]()
		{
			if (game::DoesSlotExist(slot))
			{
				application::UIState::Write(confirmState);
				return;
			}
			game::SaveToSlot(slot);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		};
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1",SlotSaver(1, ::UIState::CONFIRM_OVERWRITE_SLOT1)},
		{"2",SlotSaver(2, ::UIState::CONFIRM_OVERWRITE_SLOT2)},
		{"3",SlotSaver(3, ::UIState::CONFIRM_OVERWRITE_SLOT3)},
		{"4",SlotSaver(4, ::UIState::CONFIRM_OVERWRITE_SLOT4)},
		{"5",SlotSaver(5, ::UIState::CONFIRM_OVERWRITE_SLOT5)},
		{"6",SaveToAutosave},
		{"7",application::UIState::GoTo(::UIState::LEAVE_PLAY)}
	};

	void SaveGame::Start()
	{
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 7.",
				Refresh));
	}
}