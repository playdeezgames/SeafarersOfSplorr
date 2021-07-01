#include "Application.Command.h"
#include "Application.Renderer.h"
#include <format>
#include "Game.Avatar.Statistics.h"
#include "Visuals.Texts.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Game.Avatar.h"
#include "Common.Audio.h"
#include "Application.OnEnter.h"
#include "Visuals.Data.Colors.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::AvatarStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	const std::string TEXT_ATTACK = "Attack";
	const std::string TEXT_ATTACK_TIMER = "AttackTimer";
	const std::string TEXT_DEFEND = "Defend";
	const std::string TEXT_DEFEND_TIMER = "DefendTimer";
	const std::string TEXT_BOWEL = "Bowel";
	const std::string TEXT_DRUNKENNESS = "Drunkenness";
	const std::string TEXT_NAUSEA = "Nausea";	
	const std::string TEXT_KEYS= "Keys";
	const std::string TEXT_POOP = "Poop";
	const std::string CAN_POOP = "[Poop!]";
	const std::string CANNOT_POOP = "";
	const std::string AREA_POOP = "Poop";


	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() {application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() {application::UIState::Write(::UIState::IN_PLAY_INVENTORY); }},
		{ ::Command::NEXT, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); }},
		{ ::Command::YELLOW, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); }}
	};

	static void UpdateAttack()
	{
		auto ss = std::format("Courage: {}", game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ATTACK, ss);
	}

	static void UpdateAttackTimer()
	{
		auto value = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		std::string ss;
		if (value > 0)
		{
			ss = std::format("Duration: {}", value);
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ATTACK_TIMER, ss);
	}

	static void UpdateDefend()
	{
		std::string ss = std::format("Alertness: {}", game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DEFEND, ss);
	}

	static void UpdateDefendTimer()
	{
		auto value = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		std::string ss;
		if (value > 0)
		{
			ss = std::format("Duration: {}", value);
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DEFEND_TIMER, ss);
	}

	static void UpdateBowel()
	{
		std::string ss = std::format("Bowel: {}", game::avatar::Statistics::Read(game::avatar::Statistic::BOWEL));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_BOWEL, ss);
	}

	static void UpdateDrunkenness()
	{
		std::string ss = std::format("Drunkenness: {}", game::avatar::Statistics::Read(game::avatar::Statistic::DRUNKENNESS));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DRUNKENNESS, ss);
	}

	static void UpdateNausea()
	{
		std::string ss = std::format("Nausea: {}", game::avatar::Statistics::Read(game::avatar::Statistic::NAUSEA));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_NAUSEA, ss);
	}

	static void UpdateKeys()
	{
		std::string ss = std::format("Keys: {}", game::avatar::Statistics::Read(game::avatar::Statistic::KEYS));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_KEYS, ss);
	}

	static void UpdatePoop()
	{
		if (!game::Avatar::CanPoop())
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_POOP, CANNOT_POOP);
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_POOP, CAN_POOP);
		}
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		if (area == AREA_POOP)
		{
			visuals::Texts::SetColor(LAYOUT_NAME, TEXT_POOP, visuals::data::Colors::HIGHLIGHT);
		}
		else
		{
			visuals::Texts::SetColor(LAYOUT_NAME, TEXT_POOP, visuals::data::Colors::NORMAL);
		}
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		if (area == AREA_POOP && game::Avatar::CanPoop())
		{
			common::audio::Sfx::Play(game::Avatar::Poop(false));
			application::OnEnter::Handle();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_STATUS, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_STATUS, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));

		::application::Command::SetHandlers(::UIState::IN_PLAY_STATUS, commandHandlers);

		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATUS, LAYOUT_NAME);

		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttack);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttackTimer);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefend);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefendTimer);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateBowel);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDrunkenness);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateNausea);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateKeys);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdatePoop);
	}
}