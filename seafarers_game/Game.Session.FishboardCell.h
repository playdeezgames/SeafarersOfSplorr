#pragma once
#include "Game.Fish.h"
#include <optional>
namespace game::session
{
	struct FishboardCell
	{
		FishboardCell(int, int, int);
		std::optional<bool> IsRevealed() const;
		std::optional<game::Fish> GetFish() const;
		void Reveal() const;
	private:
		int characterId;
		int column;
		int row;
	};
}
