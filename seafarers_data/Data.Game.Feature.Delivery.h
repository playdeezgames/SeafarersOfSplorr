#pragma once
#include <vector>
namespace data::game::feature
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static void Create(int featureId, int deliveryId);
		static std::vector<int> ReadForFeature(int featureId);
		static void Clear(int deliveryId);
	};
}
