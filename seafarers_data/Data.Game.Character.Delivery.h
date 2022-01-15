#pragma once
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
	};
}
