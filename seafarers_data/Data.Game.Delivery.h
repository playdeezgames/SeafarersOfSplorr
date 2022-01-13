#pragma once
namespace data::game
{
	struct Delivery
	{
		Delivery() = delete;
		static void Initialize();
		static int Create(int fromIslandId, int toIslandId);
	};
}