#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include <Game.Ship.h>
#include <Game.ShipTypes.h>
namespace game::avatar
{
	void Ship::Reset(const game::Difficulty&)
	{
		data::game::Ship::Clear();
	}

	void Ship::Write(const Ship& ship)
	{
		data::game::avatar::Ship::Write(data::game::Player::GetAvatarId(), { ship.shipId });
		
	}

	std::optional<Ship> Ship::Read()
	{
		auto data = data::game::avatar::Ship::Read(data::game::Player::GetAvatarId());
		if (data)
		{
			Ship ship = {
				data.value().shipId
				};
			return ship;
		}
		return std::nullopt;
	}

	std::optional<double> Ship::AvailableTonnage()
	{
		auto ship = Read();
		if (ship)
		{
			auto shipType = game::Ship::GetShipType(ship.value().shipId);
			if (shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType.value()) -
					game::avatar::Items::TotalTonnage();
			}
		}
		return std::nullopt;
	}
}