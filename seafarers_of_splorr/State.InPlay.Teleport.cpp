#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Application.OnEnter.h"
#include "Data.Stores.h"
#include "Common.RNG.h"
#include "Visuals.Texts.h"
#include "Game.Achievements.h"
namespace state::in_play::Teleport
{
	const size_t TICKS_TOTAL = 3000;
	const std::string LAYOUT_NAME = "State.InPlay.Teleport";
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
			common::audio::Sfx::Play(application::UIState::EnterGame());
			ticksLeft = TICKS_TOTAL;
		}
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		RunOutTimer();
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_TELEPORT, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_TELEPORT, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_TELEPORT, RunOutTimer);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_TELEPORT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_TELEPORT, OnUpdate);
	}
}
