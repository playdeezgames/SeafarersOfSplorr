#pragma once
#include "Game.Feature.h"
namespace game::session::island
{
	struct Feature
	{
		constexpr Feature(int islandId, const game::Feature& feature) : islandId(islandId), feature(feature) {}
	private:
		int islandId;
		game::Feature feature;
	};
}
