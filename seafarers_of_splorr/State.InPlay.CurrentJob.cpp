#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Quest.h"
#include "Game.Heading.h"
#include "Game.Islands.h"
#include "Game.Islands.Quests.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Visuals.Texts.h"
namespace state::in_play::CurrentJob
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarJob";
	const std::string MENU_ID = "CurrentJob";
	const std::string MENU_ITEM_ABANDON_JOB = "Abandon";
	const std::string TEXT_LINE1 = "Line1";
	const std::string TEXT_LINE2 = "Line2";
	const std::string TEXT_LINE3 = "Line3";
	const std::string TEXT_LINE4 = "Line4";
	const std::string TEXT_LINE5 = "Line5";
	const std::string TEXT_LINE6 = "Line6";

	enum class CurrentJobMenuItem
	{
		ABANDON,
		CANCEL
	};

	const std::map<CurrentJobMenuItem, std::function<void()>> activators =
	{
		{ CurrentJobMenuItem::ABANDON, ::application::UIState::GoTo(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB) },
		{ CurrentJobMenuItem::CANCEL, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (CurrentJobMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void UpdateQuestText(const game::Quest::QuestModel& questModel)//TODO: duplicated!
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "Please deliver this");//TODO: hardcoded
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, questModel.itemName);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, std::format("to {}", questModel.personName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, std::format("the {}", questModel.professionName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, std::format("Reward: {:.2f}", questModel.reward));

		auto islandModel = game::Islands::Read(questModel.destination).value();
		double distance = game::Heading::Distance(questModel.destination, game::Avatar::GetLocation());
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, std::format("at {} ({:.2f}).", islandModel.name, distance));
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ABANDON_JOB, true);
	}

	static void UpdateNoQuestText()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "No Jobs");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, "");
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ABANDON_JOB, false);
	}

	static void UpdateText()
	{
		auto quest = game::avatar::Quest::Read();
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
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CURRENT_JOB, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CURRENT_JOB, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CURRENT_JOB, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CURRENT_JOB, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CURRENT_JOB, LAYOUT_NAME);
	}
}