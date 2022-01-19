#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <Game.Session.h>
#include "State.LoadGame.h"
#include "State.StartGame.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::LOAD_GAME;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Load Game:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		if (game::Session().GetSaves().GetSlot1().Exists())
		{
			Terminal::WriteLine("1) Slot 1");
		}
		if (game::Session().GetSaves().GetSlot2().Exists())
		{
			Terminal::WriteLine("2) Slot 2");
		}
		if (game::Session().GetSaves().GetSlot3().Exists())
		{
			Terminal::WriteLine("3) Slot 3");
		}
		if (game::Session().GetSaves().GetSlot4().Exists())
		{
			Terminal::WriteLine("4) Slot 4");
		}
		if (game::Session().GetSaves().GetSlot5().Exists())
		{
			Terminal::WriteLine("5) Slot 5");
		}
		if (game::Session().GetSaves().GetAuto().Exists())
		{
			Terminal::WriteLine("6) Autosave Slot");
		}
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();

	}

	static void GoBack()
	{
		::application::UIState::Write(StartGame::GetStateId());
	}

	static void LoadFromAutosave()
	{
		if (game::Session().GetSaves().GetAuto().Exists())
		{
			game::Session().GetSaves().GetAuto().Load();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
	}

	static void Slot1Loader()
	{
		if (game::Session().GetSaves().GetSlot1().Exists())
		{
			game::Session().GetSaves().GetSlot1().Load();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
	}

	static void Slot2Loader()
	{
		if (game::Session().GetSaves().GetSlot2().Exists())
		{
			game::Session().GetSaves().GetSlot2().Load();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
	}

	static void Slot3Loader()
	{
		if (game::Session().GetSaves().GetSlot3().Exists())
		{
			game::Session().GetSaves().GetSlot3().Load();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
	}

	static void Slot4Loader()
	{
		if (game::Session().GetSaves().GetSlot4().Exists())
		{
			game::Session().GetSaves().GetSlot4().Load();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
	}

	static void Slot5Loader()
	{
		if (game::Session().GetSaves().GetSlot5().Exists())
		{
			game::Session().GetSaves().GetSlot5().Load();
			application::UIState::Write(::UIState::TIP);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", Slot1Loader },
		{"2", Slot2Loader },
		{"3", Slot3Loader },
		{"4", Slot4Loader },
		{"5", Slot5Loader },
		{"6", LoadFromAutosave},
		{"0", GoBack}
	};

	void LoadGame::Start()
	{
		::application::Renderer::SetRenderLayout(::UIState::LOAD_GAME, Terminal::LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::LOAD_GAME, OnEnter);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}