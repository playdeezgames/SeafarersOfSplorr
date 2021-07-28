#include "Game.Avatar.Destination.h"
#include "Data.Game.Avatar.Destination.h"
namespace game::avatar::destination
{
	std::optional<common::XY<double>> GetDestination(int index)
	{
		return data::game::avatar::Destination::Read(index);
	}

	void SetDestination(int index, const std::optional<common::XY<double>>& destination)
	{
		if (destination)
		{
			data::game::avatar::Destination::Write(index, destination.value());
			return;
		}
		data::game::avatar::Destination::Clear(index);
	}
}