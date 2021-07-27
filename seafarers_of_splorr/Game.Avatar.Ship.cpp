#include "Data.Game.Avatar.Ship.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include "Game.Ships.h"
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

	double AvailableTonnage()
	{
		return 
			game::Ships::Read(Read()).properties.find(game::ship::Property::TONNAGE)->second -
			game::avatar::Items::TotalTonnage();
	}
}