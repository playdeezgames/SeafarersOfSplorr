#include "Game.Avatar.Destination.h"
#include "Data.Game.Avatar.Destination.h"
namespace game::avatar::destination
{
	std::optional<common::XY<double>> GetDestination(const game::avatar::Destination& index)
	{
		return data::game::avatar::Destination::Read((int)index);
	}

	void SetDestination(const game::avatar::Destination& index, const std::optional<common::XY<double>>& destination)
	{
		if (destination)
		{
			data::game::avatar::Destination::Write((int)index, destination.value());
			return;
		}
		data::game::avatar::Destination::Clear((int)index);
	}
}