#pragma once
namespace data::game::character
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static void Create(int characterId, int deliveryId);
		static int ReadCount(int characterId);
	};
}
