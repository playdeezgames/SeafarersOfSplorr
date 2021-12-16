#pragma once
#include <functional>
#include "Game.Characteristic.h"
#include <optional>
#include <map>
namespace game::characters
{
	struct Characteristics
	{
		static std::map<Characteristic, int> ReadLegacy(int);
		static void Generate(int);
		static std::optional<int> GetMaximumHitPoints(int);
		static std::optional<bool> Check(int, const Characteristic&);
		static std::optional<bool> OpposedCheck(int, const Characteristic&, int);
		static void OnCheck(int, const Characteristic&, std::function<void(bool)>);
		static void OnOpposedCheck(int, const Characteristic&, int, std::function<void(bool)>);
	};
}