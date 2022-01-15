#include <algorithm>
#include <Common.Heading.h>
#include <Data.Game.Character.Island.Known.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.CurrentIsland.h>
#include "Game.Session.h"
#include "Game.Session.Ship.h"
namespace game::session
{
	using ShipData = data::game::Ship;

	bool Ship::CanDock() const
	{
		return GetDockableIslands().TryGetFirst().has_value();
	}

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
		data::game::character::island::Known::Write(
			characterId,
			islandId);
		game::Session()
			.GetCharacters()
			.GetCharacter(characterId)
			.SetIsland(islandId);
	}

	void Ship::Dock() const
	{
		if (!IsDocked())
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
		if (IsDocked())
		{
			auto characters = game::Session().GetCharacters();
			auto billets = data::game::character::Ship::ReadCharactersForShip(shipId);
			if (std::all_of(
				billets.begin(),
				billets.end(),
				[characters](int characterId) {
					return characters.GetCharacter(characterId).IsOnIsland();
				}))
			{
				std::for_each(
					billets.begin(), 
					billets.end(), 
					[characters](int characterId)
					{
						characters.GetCharacter(characterId).ClearIsland();
					});
				data::game::ship::CurrentIsland::ClearForShip(shipId);
				return true;
			}
		}
		return false;
	}

	std::optional<Island> Ship::TryGetIsland() const
	{
		auto islandId = data::game::ship::CurrentIsland::ReadIslandId(shipId);
		if (islandId)
		{
			return Island(*islandId);
		}
		return std::nullopt;
	}

	bool Ship::IsDocked() const
	{
		return data::game::ship::CurrentIsland::ReadIslandId(shipId).has_value();
	}

	Island Ship::GetIsland() const
	{
		return TryGetIsland().value();
	}

	world::ShipType Ship::GetShipType() const
	{
		return world::ShipType((game::ShipType)data::game::Ship::GetShipType(shipId).value());
	}

	static double GetEffectiveSpeed(int shipId, double heading, double speed)
	{
		auto fouling = 0.0;//TODO: put fouling back!
		auto effectiveSpeed = speed * (1.0 - fouling);

		effectiveSpeed *= game::Session().GetWorld().GetWind().GetSpeedMultiplier(heading);

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