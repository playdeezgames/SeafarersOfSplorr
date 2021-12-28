#pragma once
#include "Game.Characters.PlightType.h"
#include "Game.Characters.Plight.h"
#include <string>
namespace game::session::character
{
	struct Plight
	{
		constexpr Plight(
			int characterId, 
			const game::characters::Plight& plight) 
			: characterId(characterId)
			, plight(plight) {}
		void ApplyTurnEffects() const;
		std::string GetName() const;
		game::characters::PlightType GetType() const;
	private:
		int characterId;
		game::characters::Plight plight;
	};
}
