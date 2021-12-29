#pragma once
#include "Game.Session.Island.Feature.h"
namespace game::session::island
{
	struct Features
	{
		constexpr Features(int islandId) : islandId(islandId) {}
		constexpr Feature GetFeature(const game::Feature& feature) { return Feature(islandId, feature); }
	private:
		int islandId;
	};
}
