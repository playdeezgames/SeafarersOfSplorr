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
namespace state::Tip
{
	const size_t TICKS_TOTAL = 3000;
	const std::string LAYOUT_NAME = "State.Tip";
	const std::string TEXT_TIP_1 = "Tip1";
	const std::string TEXT_TIP_2 = "Tip2";
	const std::string TEXT_TIP_3 = "Tip3";
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
			//common::audio::Sfx::Play(application::UIState::EnterGame());
			application::UIState::Write(::UIState::MAIN_MENU);//TODO: once there's a game, go there instead.
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
		auto& store = data::Stores::GetStore(data::Store::TIPS);

		auto index = common::RNG::FromRange(0u, store.size());
		auto& tip = store[index];

		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_1, tip[0]);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_2, tip[1]);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_3, tip[2]);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::TIP, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::TIP, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::TIP, RunOutTimer);
		::application::Renderer::SetRenderLayout(::UIState::TIP, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::TIP, OnUpdate);
	}
}
