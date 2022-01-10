#include "State.ScratchPad.ShipAdjective.h"
namespace state::scratch_pad
{
	std::string ShipAdjective::shipAdjective = "";

	void ShipAdjective::SetAdjective(const std::string& adjective)
	{
		shipAdjective = adjective;
	}

	const std::string& ShipAdjective::GetAdjective()
	{
		return shipAdjective;
	}
}