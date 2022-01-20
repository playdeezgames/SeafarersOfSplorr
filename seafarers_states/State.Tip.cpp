#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.RNG.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.Registrar.h"
#include "State.Terminal.h"
#include "State.Tip.h"
#include "UIState.h"
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
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	void Tip::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::Keyboard::AddHandler(stateId, OnKeyboard);
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
			});
	}
}
