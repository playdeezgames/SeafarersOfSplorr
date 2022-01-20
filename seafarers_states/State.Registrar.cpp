#include "State.Registrar.h"
#include "UIState.h"
namespace state
{
	int Registrar::stateId = (int)::UIState::REGISTRAR_STARTS_HERE;

	void Registrar::Register(std::optional<int>& stateId, std::function<void(int)> starter)
	{
		if (!stateId)
		{
			stateId = Registrar::NextStateId();
			starter(*stateId);
		}
	}
}