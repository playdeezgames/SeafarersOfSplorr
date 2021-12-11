#pragma once
namespace game::session::world
{
	struct Wind
	{
		Wind();
		double GetHeading() const;
		double GetMultiplier(double) const;
	};
}
