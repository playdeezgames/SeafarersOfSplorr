#include "State.ScratchPad.DetailedStart.Profession.h"
namespace state::scratch_pad::detailed_start
{
	game::Profession Profession::chosenProfession = game::Profession::SAILOR;

	void Profession::SetProfession(const game::Profession& profession)
	{
		chosenProfession = profession;
	}

	const game::Profession& Profession::GetProfession()
	{
		return chosenProfession;
	}
}