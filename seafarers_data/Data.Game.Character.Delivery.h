#pragma once
#include <optional>
#include <vector>
namespace data::game::character
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static void Create(int characterId, int deliveryId);
		static int ReadCount(int characterId);
		static std::vector<int> Read(int characterId);
		static void Remove(int deliveryId);
		static std::optional<int> ReadCharacterId(int deliveryId);
	};
}
