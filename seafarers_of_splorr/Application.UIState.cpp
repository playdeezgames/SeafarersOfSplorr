#include "Application.Sounds.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Game.Achievements.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include <stack>
namespace application::UIState
{
	static ::UIState uiState = ::UIState::SPLASH;
	static std::stack<::UIState> stateStack;

	void Write(const ::UIState& state)
	{
		uiState = state;
	}

	std::function<void()> GoTo(const ::UIState& state)
	{
		return [state]()
		{
			Write(state);
		};
	}


	const ::UIState& Read()
	{
		return uiState;
	}

	void Push(const ::UIState& state)
	{
		stateStack.push(uiState);
		Write(state);
	}

	std::function<void()> PushTo(const ::UIState& state)
	{
		return [state]() {
			Push(state);
		};
	}

	void Pop()
	{
		uiState = stateStack.top();
		stateStack.pop();
	}

	std::function<void()> PopFrom()
	{
		return []() {
			Pop();
		};
	}
}