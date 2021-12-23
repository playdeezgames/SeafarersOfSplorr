#include "Game.Islands.h"
#include "Game.Session.Island.h"
namespace game::session
{
	static std::optional<bool> TryIsKnown(int characterId, int islandId)
	{
		auto island = game::Islands::Read(characterId, islandId);
		if (island)
		{
			return island.value().isKnown;
		}
		return std::nullopt;
	}

	bool Island::IsKnown(int characterId) const
	{
		return TryIsKnown(characterId, islandId).value();
	}

	static std::optional<std::string> TryGetDisplayName(int characterId, int islandId)
	{
		auto island = game::Islands::Read(characterId, islandId);
		if (island)
		{
			return island.value().GetDisplayName();
		}
		return std::nullopt;
	}

	std::string Island::GetDisplayName(int characterId) const
	{
		return TryGetDisplayName(characterId, islandId).value();
	}

	static std::optional<common::XY<double>> TryGetLocation(int characterId, int islandId)
	{
		auto island = game::Islands::Read(characterId, islandId);
		if (island)
		{
			return island.value().absoluteLocation;
		}
		return std::nullopt;
	}

	common::XY<double> Island::GetLocation(int characterId) const
	{
		return TryGetLocation(characterId, islandId).value();
	}

	island::Tavern Island::GetTavern() const
	{
		return island::Tavern(islandId);
	}
}