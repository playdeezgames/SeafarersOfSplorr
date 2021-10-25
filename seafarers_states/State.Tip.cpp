#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.RNG.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.Terminal.h"
#include "State.Tip.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::TIP;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		auto& store = data::json::Stores::GetStore(data::json::Store::TIPS);

		auto index = common::RNG::FromRange(0u, store.size());
		auto& tip = store[index];

		Terminal::ClearStatusLine();
		Terminal::ClearInput();
		Terminal::SetForeground(game::Colors::BROWN);
		Terminal::WriteLine();
		Terminal::WriteLine(tip[0]);
		Terminal::WriteLine(tip[1]);
		Terminal::WriteLine(tip[2]);

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

	void Tip::Start()
	{
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKeyboard);
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
