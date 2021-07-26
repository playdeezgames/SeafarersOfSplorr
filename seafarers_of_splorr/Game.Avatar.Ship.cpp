#include "Game.Avatar.Ship.h"
#include "Game.Ships.h"
#include "Data.Game.Avatar.Ship.h"
namespace game::avatar::Ship
{
	void Reset(const game::Difficulty&)
	{
		auto ship = game::Ships::GenerateForAvatar();
		Write(ship);
	}

	void Write(const game::Ship& ship)
	{
		data::game::avatar::Ship::Write((int)ship);
	}

	game::Ship Read()
	{
		return (game::Ship)data::game::avatar::Ship::Read();
	}
}