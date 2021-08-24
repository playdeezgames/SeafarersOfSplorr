#pragma once
namespace game::avatar
{
	struct ShipStatistics
	{
		static double GetFouling();
		static double GetFoulingPercentage();
		static void IncreaseFouling(double);
	};
}