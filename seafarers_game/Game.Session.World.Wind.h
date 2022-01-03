#pragma once
namespace game::session::world
{
	struct Wind
	{
		constexpr explicit Wind(int worldId) : worldId(worldId) {};
		double GetHeading() const;
		double GetSpeedMultiplier(double heading) const;
		void SetHeading(double heading) const;
		void ApplyTurnEffects() const;
		void Reset() const {}
	private:
		int worldId;
	};
}
