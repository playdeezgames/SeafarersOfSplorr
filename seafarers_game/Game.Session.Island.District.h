#pragma once
#include "Game.Difficulty.h"
#include "Game.Island.District.h"
#include "Game.Session.Island.District.Features.h"
#include <string>
namespace game::session::island
{
	struct District
	{
		constexpr explicit District(int islandId, const game::island::District& district)
			: islandId(islandId)
			, district(district) {}
		constexpr explicit operator game::island::District() const { return district; }
		constexpr district::Features GetFeatures() const { return district::Features(islandId, district); }
		std::string GetName() const;
		void Populate(const game::Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int islandId;
		game::island::District district;
	};
}
