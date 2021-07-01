#include "Application.UIState.h"
#include "Application.Sounds.h"
#include "Game.h"
#include "Game.Achievements.h"
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
		game::AutoSave();
		return std::nullopt;
	}
}