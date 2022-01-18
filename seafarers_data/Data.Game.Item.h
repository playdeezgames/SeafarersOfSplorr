#pragma once
#include <optional>
namespace data::game
{
	struct Item//TODO: eliminate this, do not replace
	{
		Item() = delete;
		static void Initialize();
		static [[nodiscard]] int Create(int itemTypeId, int quantity);
		static std::optional<int> ReadItemTypeId(int itemId);
		static std::optional<int> ReadQuantity(int itemId);
		static void WriteQuantity(int itemId, int quantity);
		static void Purge();
	};
}
