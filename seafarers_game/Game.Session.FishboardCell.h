#pragma once
#include "Game.Fish.h"
#include <optional>
namespace game::session
{
	struct FishboardCell
	{
		FishboardCell(int, int, int);
		bool IsRevealed() const;
		game::Fish GetFish() const;
		void Reveal() const;
	private:
		int characterId;
		int column;
		int row;
	};
}
