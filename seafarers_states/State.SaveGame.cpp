#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <Game.Session.h>
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
		if (game::Session().GetSaves().GetSlot1().Exists())
		{
			Terminal::WriteLine("1) Slot 1(overwrite)");
		}
		else
		{
			Terminal::WriteLine("1) Slot 1");
		}
		if (game::Session().GetSaves().GetSlot2().Exists())
		{
			Terminal::WriteLine("2) Slot 2(overwrite)");
		}
		else
		{
			Terminal::WriteLine("2) Slot 2");
		}
		if (game::Session().GetSaves().GetSlot3().Exists())
		{
			Terminal::WriteLine("3) Slot 3(overwrite)");
		}
		else
		{
			Terminal::WriteLine("3) Slot 3");
		}
		if (game::Session().GetSaves().GetSlot4().Exists())
		{
			Terminal::WriteLine("4) Slot 4(overwrite)");
		}
		else
		{
			Terminal::WriteLine("4) Slot 4");
		}
		if (game::Session().GetSaves().GetSlot5().Exists())
		{
			Terminal::WriteLine("5) Slot 5(overwrite)");
		}
		else
		{
			Terminal::WriteLine("5) Slot 5");
		}
		if (game::Session().GetSaves().GetAuto().Exists())
		{
			Terminal::WriteLine("6) Autosave Slot(overwrite)");
		}
		else
		{
			Terminal::WriteLine("6) Autosave Slot");
		}
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void SaveToAutosave()
	{
		game::Session().GetSaves().GetAuto().Store();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Slot1Saver()
	{
		if (game::Session().GetSaves().GetSlot1().Exists())
		{
			application::UIState::Write(::UIState::CONFIRM_OVERWRITE_SLOT1);
			return;
		}
		game::Session().GetSaves().GetSlot1().Store();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Slot2Saver()
	{
		if (game::Session().GetSaves().GetSlot2().Exists())
		{
			application::UIState::Write(::UIState::CONFIRM_OVERWRITE_SLOT2);
			return;
		}
		game::Session().GetSaves().GetSlot2().Store();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Slot3Saver()
	{
		if (game::Session().GetSaves().GetSlot3().Exists())
		{
			application::UIState::Write(::UIState::CONFIRM_OVERWRITE_SLOT3);
			return;
		}
		game::Session().GetSaves().GetSlot3().Store();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Slot4Saver()
	{
		if (game::Session().GetSaves().GetSlot4().Exists())
		{
			application::UIState::Write(::UIState::CONFIRM_OVERWRITE_SLOT4);
			return;
		}
		game::Session().GetSaves().GetSlot4().Store();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Slot5Saver()
	{
		if (game::Session().GetSaves().GetSlot5().Exists())
		{
			application::UIState::Write(::UIState::CONFIRM_OVERWRITE_SLOT5);
			return;
		}
		game::Session().GetSaves().GetSlot5().Store();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", Slot1Saver },
		{"2", Slot2Saver },
		{"3", Slot3Saver },
		{"4", Slot4Saver },
		{"5", Slot5Saver },
		{"6", SaveToAutosave},
		{"0", application::UIState::GoTo(::UIState::LEAVE_PLAY)}
	};

	void SaveGame::Start()
	{
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}