#include "Application.UIState.h"
#include "Application.Sounds.h"
#include "Game.h"
#include "Game.Achievements.h"
#include "Game.Avatar.h"
namespace application::UIState
{
	static ::UIState uiState = ::UIState::SPLASH;

	void Write(const ::UIState& state)
	{
		uiState = state;
	}

	std::function<void()> GoTo(::UIState uiState)
	{
		return [uiState]() 
		{
			Write(uiState);
		};
	}


	const ::UIState& Read()
	{
		return uiState;
	}

	std::optional<std::string> EnterGame()
	{
		//check for out of turns
		//check for out of health

		game::AutoSave();
		if (game::Avatar::GetDockedLocation())
		{
			application::UIState::Write(::UIState::IN_PLAY_DOCKED);
			return std::nullopt;
		}
		application::UIState::Write(::UIState::IN_PLAY_AT_SEA);
		return std::nullopt;
	}
}