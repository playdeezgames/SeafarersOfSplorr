#include "State.ScratchPad.HeadForMark.h"
namespace state::scratch_pad
{
	std::string HeadForMark::headForMark = "";

	void HeadForMark::SetMark(const std::string& mark)
	{
		headForMark = mark;
	}

	const std::string& HeadForMark::GetMark()
	{
		return headForMark;
	}
}