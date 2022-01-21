#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.RNG.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.InPlay.Next.h"
#include "State.Registrar.h"
#include "State.Terminal.h"
#include "State.Tip.h"
namespace state
{
	std::optional<int> Tip::stateId = std::nullopt;

	static void Refresh()
	{
		auto& store = data::json::Stores::GetStore(data::json::Store::TIPS);

		auto index = common::RNG::FromRange(0u, store.size());
		auto& tip = store[index];

		Terminal::Reinitialize();
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
		application::UIState::Write(in_play::Next::GetStateId());
		return true;
	}

	void Tip::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::Keyboard::AddHandler(currentState, OnKeyboard);
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
			});
	}
}
