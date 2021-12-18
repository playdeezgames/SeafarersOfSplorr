#pragma once
namespace game::session::world
{
	struct Wind
	{
		constexpr Wind() {};
		double GetHeading() const;
		double GetMultiplier(double) const;
		void SetHeading(double) const;
		void ApplyTurnEffects() const;
	};
}
