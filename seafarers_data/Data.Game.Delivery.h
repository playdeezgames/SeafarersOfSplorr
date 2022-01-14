#pragma once
#include <optional>
namespace data::game
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static int Create(int fromIslandId, int toIslandId);
		static std::optional<int> ReadToIsland(int deliveryId);
		static std::optional<int> ReadFromIsland(int deliveryId);
	};
}