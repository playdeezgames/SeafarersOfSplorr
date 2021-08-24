#pragma once
namespace game::avatar
{
	struct ShipStatistics
	{
		static double GetFouling();
		static void IncreaseFouling(double);
	};
}