#include "State.Registrar.h"
#include "UIState.h"
namespace state
{
	int Registrar::stateId = (int)::UIState::REGISTRAR_STARTS_HERE;

	void Registrar::Register(std::optional<int>& currentState, std::function<void(int)> starter)
	{
		if (!currentState)
		{
			currentState = Registrar::NextStateId();
			starter(*currentState);
		}
	}
}