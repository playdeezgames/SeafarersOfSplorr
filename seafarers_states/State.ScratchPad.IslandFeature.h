#pragma once
namespace state::scratch_pad
{
	struct IslandFeature
	{
		static constexpr void SetFeatureId(int featureId) { islandFeatureId = featureId; }
		static constexpr int GetFeatureId() { return islandFeatureId; }
	private:
		static int islandFeatureId;
	};
}
