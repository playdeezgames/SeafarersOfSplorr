#include "State.Registrar.h"
namespace state
{
	int Registrar::stateId = 1000;//TODO: when UIState is gone, this can go back to default

	void Registrar::Register(std::optional<int>& stateId, std::function<void(int)> starter)
	{
		if (!stateId)
		{
			stateId = Registrar::NextStateId();
			starter(*stateId);
		}
	}

}