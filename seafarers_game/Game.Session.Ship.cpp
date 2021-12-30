#include <algorithm>
#include <Common.Heading.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.CurrentIsland.h>
#include "Game.Session.h"
#include "Game.Session.Ship.h"
namespace game::session
{
	using ShipData = data::game::Ship;

	static std::optional<double> TryGetHeading(int shipId)
	{
		return ShipData::GetHeading(shipId);
	}

	double Ship::GetHeading() const
	{
		return TryGetHeading(shipId).value();
	}

	static std::optional<double> TryGetSpeed(int shipId)
	{
		return ShipData::GetSpeed(shipId);
	}

	double Ship::GetSpeed() const
	{
		return TryGetSpeed(shipId).value();
	}

	static std::optional<common::XY<double>> TryGetLocation(int shipId)
	{
		return ShipData::GetLocation(shipId);
	}

	common::XY<double> Ship::GetLocation() const
	{
		return TryGetLocation(shipId).value();
	}

	void Ship::SetHeading(double heading) const
	{
		ShipData::SetHeading(shipId, heading);
	}

	void Ship::SetSpeed(double speed) const
	{
		ShipData::SetSpeed(shipId, speed);
	}
	
	static std::optional<std::string> TryGetName(int shipId)
	{
		return ShipData::GetName(shipId);
	}

	std::string Ship::GetName() const
	{
		return TryGetName(shipId).value();
	}

	void Ship::SetName(const std::string& name) const
	{
		ShipData::SetName(shipId, name);
	}

	static void DoDock(int characterId, int islandId)
	{
		data::game::character::KnownIsland::Write(
			characterId,
			islandId);
		game::Session().GetCharacters().GetCharacter(characterId).DoAction(game::characters::Action::ENTER_DOCK);
	}

	void Ship::Dock() const
	{
		if (!TryGetIsland().has_value())
		{
			auto island = GetDockableIslands().TryGetFirst();
			if (island)
			{
				auto islandId = island->operator int();
				data::game::ship::CurrentIsland::Write(shipId, islandId);
				auto characterIds = data::game::character::Ship::ReadCharactersForShip(shipId);
				std::for_each(
					characterIds.begin(),
					characterIds.end(),
					[islandId](int characterId) 
					{
						DoDock(characterId, islandId);
					});
			}
		}
	}

	bool Ship::Undock() const
	{
		if (TryGetIsland().has_value())
		{
			auto billets = data::game::character::Ship::ReadCharactersForShip(shipId);
			if (std::all_of(
				billets.begin(),
				billets.end(),
				[](int characterId) {
					return game::Session().GetCharacters().GetCharacter(characterId).GetState() == game::characters::State::DOCK;
				}))
			{
				std::for_each(
					billets.begin(), 
					billets.end(), 
					[](int characterId) 
					{
						game::Session().GetCharacters().GetCharacter(characterId).DoAction(game::characters::Action::UNDOCK);
					});
				data::game::ship::CurrentIsland::Clear(shipId);
			}
		}
		return false;
	}

	std::optional<Island> Ship::TryGetIsland() const
	{
		auto islandId = data::game::ship::CurrentIsland::Read(shipId);
		if (islandId)
		{
			return Island(*islandId);
		}
		return std::nullopt;
	}

	Island Ship::GetIsland() const
	{
		return TryGetIsland().value();
	}

	world::ShipType Ship::GetShipType() const
	{
		return (game::ShipType)data::game::Ship::GetShipType(shipId).value();
	}

	static double GetEffectiveSpeed(int shipId, double heading, double speed)
	{
		auto fouling = 0.0;
		auto effectiveSpeed = speed * (1.0 - fouling);

		effectiveSpeed *= game::Session().GetWorld().GetWind().GetMultiplier(heading);

		effectiveSpeed *= game::Session().GetShips().GetShip(shipId).GetShipType().GetSpeedFactor();

		return effectiveSpeed;
	}

	void Ship::ApplyTurnEffects() const
	{
		auto location = ShipData::GetLocation(shipId);
		if (location)
		{
			auto heading = ShipData::GetHeading(shipId).value();
			auto effectiveSpeed = GetEffectiveSpeed(shipId, heading, ShipData::GetSpeed(shipId).value());
			location.value() +=
				common::Heading::DegreesToXY(heading) *
				effectiveSpeed;

			game::Session().GetWorld().GetBounds().ClampLocation(location.value());

			//TODO: Handle Fouling

			ShipData::SetLocation(shipId, location.value());
		}
	}


}