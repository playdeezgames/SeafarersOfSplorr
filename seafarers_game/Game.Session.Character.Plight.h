#pragma once
#include "Game.Characters.Plight.h"
namespace game::session::character
{
	struct Plight
	{
		constexpr Plight(
			int characterId, 
			const game::characters::Plight& plight) 
			: characterId(characterId)
			, plight(plight) {}
	private:
		int characterId;
		game::characters::Plight plight;
	};
}
