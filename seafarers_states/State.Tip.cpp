#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.Update.h>
#include <Application.UIState.h>
#include <Common.RNG.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Game.Achievements.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Texts.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::TIP;
	static const size_t TICKS_TOTAL = 3000;
	static const std::string LAYOUT_NAME = "State.Tip";
	static const std::string TEXT_TIP_1 = "Tip1";
	static const std::string TEXT_TIP_2 = "Tip2";
	static const std::string TEXT_TIP_3 = "Tip3";
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
			game::Statistics::Increment(game::Statistic::GAMES_PLAYED);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			ticksLeft = TICKS_TOTAL;
		}
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		RunOutTimer();
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		auto& store = data::json::Stores::GetStore(data::json::Store::TIPS);

		auto index = common::RNG::FromRange(0u, store.size());
		auto& tip = store[index];

		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_1, tip[0]);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_2, tip[1]);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_3, tip[2]);
	}

	static bool OnKeyboard(const std::string&)
	{
		RunOutTimer();
		return true;
	}

	void Tip::Start()
	{
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKeyboard);
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, OnMouseButtonUp);
		::application::Command::SetHandler(CURRENT_STATE, RunOutTimer);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Update::AddHandler(CURRENT_STATE, OnUpdate);
	}
}
