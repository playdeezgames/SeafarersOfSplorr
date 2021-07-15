#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Texts.h"
#include "Data.Stores.h"
#include "Game.Avatar.h"
#include <format>
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include "Game.Heading.h"
namespace state::in_play::IslandJobs
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandJobs";
	const std::string MENU_ID = "AcceptJob";
	const std::string TEXT_LINE1 = "Line1";
	const std::string TEXT_LINE2 = "Line2";
	const std::string TEXT_LINE3 = "Line3";
	const std::string TEXT_LINE4 = "Line4";
	const std::string TEXT_LINE5 = "Line5";
	const std::string TEXT_LINE6 = "Line6";

	enum class AcceptJobMenuItem
	{
		ACCEPT,
		CANCEL
	};

	static void OnAccept()
	{
		return;
	}

	const std::map<AcceptJobMenuItem, std::function<void()>> activators =
	{
		{ AcceptJobMenuItem::ACCEPT, OnAccept },
		{ AcceptJobMenuItem::CANCEL, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (AcceptJobMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void UpdateQuestText(const game::Quest::QuestModel& questModel)
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "Please deliver this");//TODO: hardcoded
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, questModel.itemName);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, std::format("to {}", questModel.personName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, std::format("the {}", questModel.professionName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, std::format("Reward: {:.2f}", questModel.reward));

		auto islandModel = game::Islands::Read(questModel.destination).value();
		double distance = game::Heading::Distance(questModel.destination, game::Avatar::GetLocation());
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, std::format("at {} ({:.2f}).", islandModel.name, distance));
	}

	static void UpdateNoQuestText()
	{
		return;
	}

	static void UpdateText()
	{
		auto location = game::Avatar::GetDockedLocation().value();
		auto quest = game::islands::Quests::Read(location);
		if (quest)
		{
			UpdateQuestText(quest.value());
			return;
		}
		UpdateNoQuestText();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateText();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_JOBS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_JOBS, LAYOUT_NAME);
	}
}