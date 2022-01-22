#pragma once
namespace game::session::world
{
	struct Wind
	{
		Wind() = delete;
		static double GetHeading();
		static double GetSpeedMultiplier(double heading);
		static void SetHeading(double heading);
		static void ApplyTurnEffects();
	};
}
