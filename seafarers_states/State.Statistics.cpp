#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Achievements.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <format>
#include <tuple>
#include "State.Terminal.h"
#include "States.h"
#include "UIState.h"
#include <Visuals.Texts.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::STATISTICS;
	static const std::string LAYOUT_NAME = "State.Statistics";

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Statistics:");
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::DARK_GRAY);
		Terminal::WriteLine("TODO: put statistics here!");
		application::UIState::Write(::UIState::MAIN_MENU);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	void Statistics::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
