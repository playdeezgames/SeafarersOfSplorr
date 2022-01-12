#pragma once
#include <string>
namespace game::session::island
{
	struct Feature
	{
		constexpr explicit Feature(int featureId) : featureId(featureId) {}
		static constexpr Feature FromFeatureId(int featureId) { return Feature(featureId); }
		constexpr explicit operator int() const { return featureId; }
		std::string GetName() const;
	private:
		int featureId;
	};
}
