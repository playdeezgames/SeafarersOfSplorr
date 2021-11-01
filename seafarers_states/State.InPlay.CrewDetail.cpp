#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Colors.h>
#include "State.InPlay.CrewDetail.h"
#include "State.Terminal.h"
#include <string>
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL;

	static int currentAvatarId;

	void CrewDetail::SetAvatarId(int avatarId)
	{
		currentAvatarId = avatarId;
	}

	int CrewDetail::GetAvatarId()
	{
		return currentAvatarId;
	}

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST);

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", game::Avatar::GetName(currentAvatarId).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Statistics");
		Terminal::WriteLine("2) Equipment");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", application::UIState::GoTo(::UIState::IN_PLAY_AVATAR_STATUS) },
		{ "2", application::UIState::GoTo(::UIState::IN_PLAY_EQUIPMENT)},
		{ "0", OnLeave}
	};

	void CrewDetail::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}