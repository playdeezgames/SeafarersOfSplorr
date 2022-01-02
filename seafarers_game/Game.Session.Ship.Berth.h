#pragma once
#include "Game.BerthType.h"
#include <optional>
namespace game::session::ship
{
	struct Berth
	{
		constexpr explicit Berth(int characterId) : characterId(characterId) {}
		constexpr int GetCharacterId() const { return characterId; }
		BerthType GetBerthType() const;
	private:
		int characterId;
	};
}
