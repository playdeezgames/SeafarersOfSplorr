#pragma once
#include <Common.XY.h>
#include "Game.Avatar.Destination.h"
#include <list>
#include <optional>
namespace game::avatar
{
	struct Destinations
	{
		static const std::list<game::avatar::Destination>& All();
		static std::optional<common::XY<double>> GetDestination(const game::avatar::Destination&);
		static void SetDestination(const game::avatar::Destination&, const std::optional<common::XY<double>>&);
	};
}