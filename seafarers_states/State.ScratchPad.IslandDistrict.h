#pragma once
#include <Game.Island.District.h>
namespace state::scratch_pad
{
	struct IslandDistrict
	{
		static constexpr void SetDistrict(const game::island::District& district) { currentDistrict = district; }
		static constexpr const game::island::District& GetDistrict() { return currentDistrict; }
	private:
		static game::island::District currentDistrict;
	};
}
