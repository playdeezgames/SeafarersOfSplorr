#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.InPlay.Message.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MESSAGE;

	static void Refresh()
	{
		if (!visuals::Messages::HasMessage())
		{
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		Terminal::Reinitialize();

		auto message = visuals::Messages::Read();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(message.caption);
		for (auto& detail : message.details)
		{
			Terminal::SetForeground(detail.color);
			Terminal::Write(detail.text);
		}
		Terminal::WriteLine();

		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Press any key . . .");

	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static bool OnKeyboard(const std::string&)
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	void Message::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKeyboard);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
	}
}
