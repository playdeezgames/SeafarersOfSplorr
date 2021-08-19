#include "Application.UIState.h"
#include <stack>
namespace application::UIState
{
	std::optional<int> finalState;

	void SetFinalState(int s)
	{
		finalState = s;
	}

	static int uiState = 0;//TODO: we may need to set this!
	static std::stack<int> stateStack;

	void Write(int state)
	{
		uiState = state;
	}

	std::function<void()> GoTo(int state)
	{
		return [state]()
		{
			Write(state);
		};
	}

	int Read()
	{
		return uiState;
	}

	void Push(int state)
	{
		stateStack.push(uiState);
		Write(state);
	}

	std::function<void()> PushTo(int state)
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