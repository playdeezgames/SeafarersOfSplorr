#pragma once
#include "Game.Session.Item.Type.h"
#include <optional>
namespace game::session
{
	struct Item
	{
		constexpr explicit Item(int itemId) : itemId(itemId) {}
		item::Type GetItemType() const;
		void SetQuantity(int quantity) const;
		int GetQuantity() const;
		std::optional<int> TryGetCharacterId() const;
		int GetCharacterId() const;
		void SetCharacterId(int) const;
	private:
		int itemId;
	};
}