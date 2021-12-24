#pragma once
#include <optional>
namespace data::game
{
	struct Item
	{
		static void Initialize();
		static int Create(int, int);
		static std::optional<int> ReadItemTypeId(int);
		static std::optional<int> ReadQuantity(int);
		static void WriteQuantity(int, int);
		static void Purge();
	};
}
