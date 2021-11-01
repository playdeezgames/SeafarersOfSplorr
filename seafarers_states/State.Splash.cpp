#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.Update.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include "State.Splash.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::SPLASH;
	static const std::string LAYOUT_NAME = "State.Splash";
	static const size_t TICKS_TOTAL = 3000;
	static size_t ticksLeft = TICKS_TOTAL;

	static void RunOutTimer()
	{
		ticksLeft = 0;
	}

	static void OnUpdate(const unsigned int& ticks)
	{
		ticksLeft = (ticksLeft > ticks) ? (ticksLeft - ticks) : (0);
		if (ticksLeft == 0)
		{
			::application::UIState::Write(::UIState::MAIN_MENU);
			ticksLeft = TICKS_TOTAL;
		}
	}

	static bool OnKeyboard(const std::string&)
	{
		RunOutTimer();
		return true;
	}

	void Splash::Start()
	{
		application::OnEnter::AddHandler(CURRENT_STATE, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		application::Keyboard::AddHandler(CURRENT_STATE, OnKeyboard);
		application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		application::Update::AddHandler(CURRENT_STATE, OnUpdate);
	}
}
