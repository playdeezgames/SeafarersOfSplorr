#pragma once
#include <Common.XY.h>
#include "Game.Avatar.Destination.h"
#include <list>
#include <optional>
#include <string>
namespace game::avatar//20211017
{
	struct Destinations
	{
		static const std::list<Destination>& All();

		static std::optional<common::XY<double>> ReadLocation(const Destination&);
		static void WriteLocation(const Destination&, const std::optional<common::XY<double>>&);

		static std::optional<std::string> ReadName(const Destination&);
		static void WriteName(const Destination&, const std::string&);
	};
}
