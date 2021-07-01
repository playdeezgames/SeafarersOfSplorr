#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Visuals.Texts.h"
#include "Game.Avatar.Items.h"
#include "States.h"
#include "Application.MouseButtonUp.h"
#include "Visuals.Layouts.h"
#include "Game.Avatar.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include <format>
#include "Game.Achievements.h"
#include "Game.Avatar.Statistics.h"
namespace state::in_play::Dead
{
	const std::string LAYOUT_NAME = "State.InPlay.Dead";
	const std::string TEXT_JOOLS = "Jools";
	const std::string TEXT_TROUSERS = "Trousers";
	const std::string TEXT_KILLED_BY = "KilledBy";

	static void OnEnter()
	{
		std::string ss = std::format("You collected {} jools!", game::avatar::Items::Read(game::Avatar::GetDescriptor().joolsItemId));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_JOOLS, ss);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_KILLED_BY, std::format("Killed by: {}", game::avatar::Statistics::GetCauseOfDeath()));
		if (game::avatar::Items::Read(game::Avatar::GetDescriptor().trousersItemId)>0)
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_TROUSERS, "...at least you kept yer dignity!");
			game::Statistics::Increment(game::Statistic::DIED_WITH_DIGNITY);
		}
		else if (game::avatar::Items::Read(game::Avatar::GetDescriptor().soiledTrousersItemId) > 0)
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_TROUSERS, "...and you pooped yer pants!");
			game::Statistics::Increment(game::Statistic::DIED_WITHOUT_DIGNITY);
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_TROUSERS, "...you lost yer trousers(again)!");
			game::Statistics::Increment(game::Statistic::DIED_WITHOUT_DIGNITY);
		}
		game::audio::Mux::Play(game::audio::Mux::Theme::DEATH);
	}

	bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DEAD, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_DEAD, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DEAD, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DEAD, OnEnter);
	}
}
