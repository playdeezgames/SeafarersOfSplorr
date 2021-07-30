#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Game.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Menus.h"
#include "Visuals.Areas.h"
namespace state::Start
{
	const std::string LAYOUT_NAME = "State.Start";
	const std::string MENU_ID = "Start";

	enum class StartGameItem
	{
		CONTINUE_GAME,
		NEW_GAME_EASY,
		NEW_GAME_NORMAL,
		NEW_GAME_HARD,
		NEW_GAME_HARDCORE,
		BACK
	};

	static std::function<void()> NewGame(const game::Difficulty& difficulty)
	{
		return [difficulty]()
		{
			game::Reset(difficulty);
			application::UIState::Write(::UIState::TIP);
		};
	}

	const std::map<StartGameItem, std::function<void()>> activators =
	{
		{ StartGameItem::NEW_GAME_EASY, NewGame(game::Difficulty::EASY) },
		{ StartGameItem::NEW_GAME_NORMAL, NewGame(game::Difficulty::NORMAL) },
		{ StartGameItem::NEW_GAME_HARD, NewGame(game::Difficulty::HARD) },
		{ StartGameItem::NEW_GAME_HARDCORE, NewGame(game::Difficulty::HARDCORE) },
		{ StartGameItem::CONTINUE_GAME, ::application::UIState::GoTo(::UIState::LOAD_GAME) },
		{ StartGameItem::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (StartGameItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::START_GAME, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::START_GAME, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::START_GAME, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::START_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::START_GAME, LAYOUT_NAME);
	}
}