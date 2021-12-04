#pragma once
#include "Game.Characteristic.h"
#include <optional>
#include <map>
namespace game::characters
{
	struct Characteristics
	{
		static void Write(int, const Characteristic&, int);
		static std::optional<int> Read(int, const Characteristic&);
		static std::map<Characteristic, int> Read(int);
		static void Generate(int);
		static std::optional<int> GetMaximumHitPoints(int);
	};
}