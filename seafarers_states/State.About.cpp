#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Common.RNG.h>
#include <format>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Terminals.h>
namespace state
{
	static const std::string LAYOUT_NAME = "State.Terminal";
	static const ::UIState CURRENT_STATE = ::UIState::ABOUT;
	static const std::string TERMINAL_ID = "terminal";

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "About Seafarers of SPLORR!!");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "A presentation of TheGrumpyGameDev");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "0) Exit");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, ">");
	}

	static const std::string KEY_RETURN = "Return";
	static const std::string KEY_0 = "0";

	static bool OnKeyDown(const std::string& key)
	{
		if (key == KEY_RETURN)
		{
			application::UIState::Write(::UIState::MAIN_MENU);
			visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
			return true;
		}
		else if (key == KEY_0)
		{
			visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, key);
			return true;
		}
		return false;
	}

	void About::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKeyDown);
	}
}
