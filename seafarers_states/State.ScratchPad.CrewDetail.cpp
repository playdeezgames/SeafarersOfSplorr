#include "State.ScratchPad.CrewDetail.h"
namespace state::scratch_pad
{
	int CrewDetail::crewDetailCharacterId = 0;

	void CrewDetail::SetCharacterId(int characterId)
	{
		crewDetailCharacterId = characterId;
	}

	int CrewDetail::GetCharacterId()
	{
		return crewDetailCharacterId;
	}
}