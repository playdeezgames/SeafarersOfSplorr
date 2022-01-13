#pragma once
namespace data::game::feature
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static void Create(int featureId, int deliveryId);
	};
}
