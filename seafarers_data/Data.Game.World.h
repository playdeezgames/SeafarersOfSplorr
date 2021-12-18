#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	struct World
	{
		int version;
		common::XY<double> size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
		double windHeading;
		int currencyItemSubtypeId;

		//DEPRECATE THESE
		double unfoulingLaborMultiplier;
		double reputationReward;
		double reputationPenalty;

		static void Write(const World&);
		static std::optional<World> Read();
	};
}