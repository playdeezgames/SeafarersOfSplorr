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

	void Ship::Write(int shipId)
	{
		data::game::avatar::Ship::Write(data::game::Player::GetAvatarId(), shipId);
		
	}

	std::optional<int> Ship::Read()
	{
		return data::game::avatar::Ship::Read(data::game::Player::GetAvatarId());
	}

	std::optional<double> Ship::AvailableTonnage()
	{
		auto shipId = Read();
		if (shipId)
		{
			auto shipType = game::Ship::GetShipType(shipId.value());
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