#pragma once
#include "Game.Session.FishboardCell.h"
#include <optional>
namespace game::session
{
	struct Fishboard
	{
		Fishboard(int);
		FishboardCell GetCell(int, int) const;
		double GetProgressPercentage() const;
	private:
		int characterId;
	};
}
