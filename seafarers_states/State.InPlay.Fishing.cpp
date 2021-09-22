#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Plights.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Fishboard.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Fishboard.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Menus.h>
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play
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

	enum class StatusMenuItem
	{
		MORE_BAIT,
		GO_BACK
	};

	static void Refresh();
	static void OnMoreBait()
	{
		game::Fishboard::AddBait();
		Refresh();
	}
	static void OnGoBack()
	{
		if (game::Fishboard::IsRevealed() || game::Fishboard::HasGivenUp())
		{
			game::Avatar::DoAction(game::avatar::Action::STOP_FISHING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		game::Fishboard::GiveUp();
		Refresh();
	}

	static const std::map<StatusMenuItem, std::function<void()>> activators =
	{
		{ StatusMenuItem::GO_BACK, OnGoBack },
		{ StatusMenuItem::MORE_BAIT, OnMoreBait }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static void RefreshGuesses()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_GUESSES, std::format("{}", game::Fishboard::ReadGuesses()));
	}

	static void RefreshProgress()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_PROGRESS, std::format("{:.0f}%", game::Fishboard::ReadProgressPercentage()));
	}

	static bool ShouldShowMoreBaitMenuItem()
	{
		return 
			(!game::Fishboard::HasGuessesLeft()) && 
			!game::Fishboard::IsRevealed() && 
			!game::Fishboard::HasGivenUp() &&
			game::avatar::Items::Has(game::Item::BAIT);
	}

	static void RefreshMenu()
	{
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_MORE_BAIT, ShouldShowMoreBaitMenuItem());
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_GO_BACK, 
			(game::Fishboard::IsRevealed()) ? ("Done!") :
			(game::Fishboard::HasGivenUp()) ? ("Go Back") :
			("Give Up!"));
	}

	static void Refresh()
	{
		RefreshMenu();
		RefreshGuesses();
		RefreshProgress();
	}

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

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		if (areaName == AREA_FISHBOARD)
		{
			visuals::Fishboard::HandleMouseMotion(LAYOUT_NAME, FISHBOARD_ID, location);
		}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& location)
	{
		//TODO: either do something here, or get rid of it!
	}

	static bool OnMouseButtonUp(const std::string& areaName)
	{
		if (areaName == AREA_FISHBOARD)
		{
			auto cursor = visuals::Fishboard::ReadCursor(LAYOUT_NAME, FISHBOARD_ID);
			game::Fishboard::Reveal(cursor);
			Refresh();
		}
		return false;
	}


	void Fishing::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUp));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
