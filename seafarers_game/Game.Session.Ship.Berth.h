#pragma once
#include "Game.BerthType.h"
#include <optional>
namespace game::session::ship
{
	struct Berth
	{
		Berth(int);
		int GetCharacterId() const;
		BerthType GetBerthType() const;
	private:
		int characterId;
	};
}
