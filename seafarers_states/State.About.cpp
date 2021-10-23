#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Terminals.h>
namespace state
{
	static const std::string LAYOUT_NAME = "State.About";
	static const ::UIState CURRENT_STATE = ::UIState::ABOUT;
	static const std::string TERMINAL_ID = "terminal";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "ohai!ohai!ohai!ohai!ohai!ohai!ohai!ohai!ohai!ohai!_");
	}

	void About::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, OnMouseButtonUp);
		::application::Command::SetHandler(CURRENT_STATE, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
