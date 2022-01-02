#pragma once
#include "Game.Session.Character.Plight.h"
#include <list>
namespace game::session::character
{
	struct Plights
	{
		constexpr explicit Plights(int characterId) : characterId(characterId) {}
		constexpr Plight GetPlight(const game::characters::Plight& plight) const { return Plight(characterId, plight); }
		std::list<Plight> GetAll() const;
		void ApplyTurnEffects() const;
		bool Has(const game::characters::Plight& plight) const;
	private:
		int characterId;
	};
}
