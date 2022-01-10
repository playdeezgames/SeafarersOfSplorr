#pragma once
#include <Game.Profession.h>
namespace state::scratch_pad::detailed_start
{
	struct Profession
	{
		Profession() = delete;
		static void SetProfession(const game::Profession& profession);
		static const game::Profession& GetProfession();
	private:
		static game::Profession chosenProfession;
	};
}
