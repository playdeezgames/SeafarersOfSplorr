#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Plights.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Fishboard.h>
#include <Game.FishGame.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "State.InPlay.Fishing.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Fishboard.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Menus.h>
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play//20211020
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_FISHING;

	static const std::string LAYOUT_NAME = "State.InPlay.Fishing";
	static const std::string FONT_DEFAULT = "default";
	static const std::string MENU_ID = "Fishing";
	static const std::string MENU_ITEM_GO_BACK = "GoBack";
	static const std::string MENU_ITEM_MORE_BAIT = "MoreBait";
	static const std::string FISHBOARD_ID = "Fishboard";
	static const std::string AREA_FISHBOARD = "Fishboard";
	static const std::string TEXT_GUESSES = "Guesses";
	static const std::string TEXT_PROGRESS = "Progress";
	static const std::string TEXT_BAIT = "Bait";

	enum class StatusMenuItem
	{
		MORE_BAIT,
		GO_BACK
	};

	static bool ShouldShowMoreBaitMenuItem()
	{
		return
			!game::FishGame::HasGuessesLeft() &&
			game::avatar::Items::Has(game::Item::BAIT);
	}

	static const std::map<game::FishGameState, std::string> goBackMenuItemTexts =
	{
		{game::FishGameState::DONE, "Done!"},
		{game::FishGameState::GAVE_UP, "Go Back"},
		{game::FishGameState::FISHING, "Give Up!"},
		{game::FishGameState::OUT_OF_GUESSES, "Give Up!"}
	};

	static std::string GetGoBackMenuItemText()
	{
		return goBackMenuItemTexts.find(game::FishGame::GetState())->second;
	}
	static void RefreshMenu()
	{
		visuals::MenuItems::SetEnabled(
			LAYOUT_NAME, 
			MENU_ITEM_MORE_BAIT, 
			ShouldShowMoreBaitMenuItem());
		visuals::MenuItems::SetText(
			LAYOUT_NAME, 
			MENU_ITEM_GO_BACK,
			GetGoBackMenuItemText());
	}

	static void RefreshGuesses()
	{
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_GUESSES,
			"{}",
			game::FishGame::ReadGuesses());
	}

	static void RefreshProgress()
	{
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_PROGRESS,
			"{:.0f}%",
			game::Fishboard::ReadProgressPercentage());
	}

	static void RefreshBait()
	{
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_BAIT,
			"{}",
			(int)game::avatar::Items::Read(game::Item::BAIT));
	}

	static void Refresh()
	{
		RefreshMenu();
		RefreshGuesses();
		RefreshProgress();
		RefreshBait();
	}

	static void OnMoreBait()
	{
		game::FishGame::AddBait();
		Refresh();
	}

	static void OnDoneGoBack()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::STOP_FISHING);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnGiveUp()
	{
		game::FishGame::GiveUp();
		Refresh();
	}

	static const std::map<game::FishGameState, std::function<void()>> handleGoBacks =
	{
		{game::FishGameState::DONE, OnDoneGoBack},
		{game::FishGameState::FISHING, OnGiveUp},
		{game::FishGameState::GAVE_UP, OnDoneGoBack},
		{game::FishGameState::OUT_OF_GUESSES, OnGiveUp}
	};

	static void OnGoBack()
	{
		handleGoBacks.find(game::FishGame::GetState())->second();
	}

	static const std::map<StatusMenuItem, std::function<void()>> activators =
	{
		{ StatusMenuItem::GO_BACK, OnGoBack },
		{ StatusMenuItem::MORE_BAIT, OnMoreBait }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, OnGoBack },
		{ ::Command::RED, OnGoBack }
	};

	static void HandleMotionInFishboard(const common::XY<int>& location)
	{
		visuals::Fishboard::HandleMouseMotion(LAYOUT_NAME, FISHBOARD_ID, location);
	}

	static std::map<std::string, std::function<void(const common::XY<int>&)>> motionHandlers =
	{
		{ AREA_FISHBOARD, HandleMotionInFishboard}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		common::utility::Dispatcher::DispatchParameter(motionHandlers, areaName, location);
	}

	static void OnMouseButtonUpInFishboard()
	{
		auto cursor = visuals::Fishboard::ReadCursor(LAYOUT_NAME, FISHBOARD_ID);
		game::Fishboard::Reveal(cursor);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> buttonHandlers =
	{
		{AREA_FISHBOARD, OnMouseButtonUpInFishboard}
	};

	void Fishing::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::MouseButtonUp::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMenuMouseButtonUp(
				LAYOUT_NAME, 
				ActivateItem));
		::application::MouseButtonUp::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMouseButtonUp(
				LAYOUT_NAME, 
				common::utility::Dispatcher::DoDispatch(
					buttonHandlers, 
					true, 
					false)));
		::application::MouseMotion::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMenuMouseMotion(
				LAYOUT_NAME));
		::application::MouseMotion::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMouseMotion(
				LAYOUT_NAME, 
				OnMouseMotionInArea));
		::application::Command::SetHandlers(
			CURRENT_STATE, 
			commandHandlers);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
	}
}
