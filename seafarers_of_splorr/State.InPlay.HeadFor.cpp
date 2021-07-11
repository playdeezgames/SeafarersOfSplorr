#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::HeadFor
{
	const std::string LAYOUT_NAME = "State.InPlay.HeadFor";

	static void ReturnToGame()
	{
		::application::UIState::EnterGame();
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		ReturnToGame();
		return true;
	}

	static void ActivateItem()
	{

	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		//{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		//{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, ReturnToGame },
		{ ::Command::RED, ReturnToGame },
		{ ::Command::GREEN, ActivateItem }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_HEAD_FOR, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_HEAD_FOR, OnMouseButtonUp);
		::application::Command::SetHandlers(::UIState::IN_PLAY_HEAD_FOR, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_HEAD_FOR, LAYOUT_NAME);
	}
}
