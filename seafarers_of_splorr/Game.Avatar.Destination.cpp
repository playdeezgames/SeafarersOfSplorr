#include "Game.Avatar.Destination.h"
#include "Data.Game.Avatar.Destination.h"
namespace game::avatar::destination
{
	std::optional<common::XY<double>> GetDestination()
	{
		return data::game::avatar::Destination::Read();
	}

	void SetDestination(const std::optional<common::XY<double>>& destination)
	{
		if (destination)
		{
			data::game::avatar::Destination::Write(destination.value());
			return;
		}
		data::game::avatar::Destination::Clear();
	}
}