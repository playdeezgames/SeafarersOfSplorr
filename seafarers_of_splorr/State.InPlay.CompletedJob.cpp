#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::CompletedJob
{
	const size_t TICKS_TOTAL = 2000;
	const std::string LAYOUT_NAME = "State.InPlay.CompletedJob";
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
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
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
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_COMPLETED_JOB, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_COMPLETED_JOB, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_COMPLETED_JOB, RunOutTimer);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMPLETED_JOB, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMPLETED_JOB, OnUpdate);
	}
}
