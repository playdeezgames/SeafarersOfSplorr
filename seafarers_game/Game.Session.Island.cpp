#include "Game.Islands.h"
#include "Game.Session.Island.h"
namespace game::session
{
	Island::Island(int islandId)
		: islandId(islandId)
	{

	}

	static std::optional<bool> TryIsKnown(int islandId)
	{
		auto island = game::Islands::Read(islandId);
		if (island)
		{
			return island.value().IsKnown();
		}
		return std::nullopt;
	}

	bool Island::IsKnown() const
	{
		return TryIsKnown(islandId).value();
	}

	static std::optional<std::string> TryGetDisplayName(int islandId)
	{
		auto island = game::Islands::Read(islandId);
		if (island)
		{
			return island.value().GetDisplayName();
		}
		return std::nullopt;
	}

	std::string Island::GetDisplayName() const
	{
		return TryGetDisplayName(islandId).value();
	}

	static std::optional<common::XY<double>> TryGetLocation(int islandId)
	{
		auto island = game::Islands::Read(islandId);
		if (island)
		{
			return island.value().absoluteLocation;
		}
		return std::nullopt;
	}

	common::XY<double> Island::GetLocation() const
	{
		return TryGetLocation(islandId).value();
	}
}