#include "Data.Game.Island.h"
namespace data::game::Island
{
	void Write(const IslandData&)
	{
		//TODO: write to db
	}

	std::optional<IslandData> Read(const common::XY<double>&)
	{
		//TODO: try to read the db
		return std::nullopt;
	}

	std::list<IslandData> All()
	{
		//TODO: try to read the db
		return {};
	}
}