#pragma once
#include <optional>
namespace data::game
{
	struct ItemInstance
	{
		static int Create(int, int, int);
		static std::optional<int> ReadType(int);
		static std::optional<int> ReadSubtype(int);
		static std::optional<int> ReadQuantity(int);
		static void WriteQuantity(int, int);
		static void Purge();
	};
}
