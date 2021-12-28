#include <Common.Heading.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Island.h>
#include "Game.Characters.Ships.h"
#include "Game.Islands.h"
#include "Game.Session.h"
#include "Game.Ship.h"
#include "Game.Ship.Crew.h"
namespace game
{
	const std::string Islands::UNKNOWN = "????";

	static void KnowIsland(
		int characterId,
		std::list<Island>& accumulator, 
		int islandId, 
		const common::XY<double>& shipLocation)
	{
		data::game::character::KnownIsland::Write(characterId, islandId);
		accumulator.push_back(
			{
				islandId,
				(data::game::Island::ReadLocation(islandId).value() - shipLocation),
				data::game::Island::ReadLocation(islandId).value(),
				data::game::Island::ReadName(islandId).value(),
				true,
				data::game::Island::ReadPatronDemigodId(islandId).value()
			});
	}

	static void KnowIslandWhenCloseEnough(int characterId, std::list<Island>& accumulator, int islandId, const common::XY<double>& shipLocation, std::function<bool(int, double)> filter)
	{
		auto location = data::game::Island::ReadLocation(islandId).value();
		auto distance = common::Heading::Distance(shipLocation, location);
		if (filter(islandId, distance))
		{
			KnowIsland(characterId, accumulator, islandId, shipLocation);
		}
	}

	static std::list<Island> GetIslandsInRange(int shipId, std::function<bool(int, double)> filter)
	{
		std::list<Island> accumulator;
		auto shipLocation = game::Ship::GetLocation(shipId).value();;
		auto islands = data::game::Island::All();
		auto crew = game::ship::Crew::ReadForShip(shipId);
		for (auto crewMember : crew)
		{
			for (auto& islandId : islands)
			{
				KnowIslandWhenCloseEnough(crewMember.avatarId, accumulator, islandId, shipLocation, filter);
			}
		}
		return accumulator;
	}

	static std::function<bool(int, double)> FixedDistance(double maximumDistance)
	{
		return [maximumDistance](int, double distance)
		{
			return distance <= maximumDistance;
		};
	}

	static Island ToIsland(int islandId, bool isKnown)
	{
		return 
		{
			islandId,
			{0.0, 0.0},
			data::game::Island::ReadLocation(islandId).value(),
			data::game::Island::ReadName(islandId).value(),
			isKnown,
			data::game::Island::ReadPatronDemigodId(islandId).value()
		};
	}

	static void ObfuscateIfUnknown(int characterId, game::Island& island)
	{
		auto islandId = island.id;
		if (!data::game::character::KnownIsland::Read(characterId, islandId))
		{
			island.name = Islands::UNKNOWN;
		}
	}
}
