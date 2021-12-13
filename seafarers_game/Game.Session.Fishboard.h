#pragma once
#include "Game.Session.FishboardCell.h"
#include <optional>
namespace game::session
{
	struct Fishboard
	{
		Fishboard(int);
		std::optional<FishboardCell> GetCell(int, int) const;
		std::optional<double> GetProgressPercentage() const;
	private:
		int characterId;
	};
}
