#pragma once
#include <optional>
namespace data::game
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static int Create(int fromIslandId, int toIslandId, int timeLimit);
		static std::optional<int> ReadToIsland(int deliveryId);
		static std::optional<int> ReadFromIsland(int deliveryId);
		static std::optional<int> ReadTimeLimit(int deliveryId);
		static void WriteTimeLimit(int deliveryId, int timeLimit);
		static void Remove(int deliveryId);
	};
}