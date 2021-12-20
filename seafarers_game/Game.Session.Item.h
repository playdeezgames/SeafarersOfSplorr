#pragma once
#include "Game.Session.Item.Type.h"
#include <optional>
namespace game::session
{
	struct Item
	{
		constexpr Item(int itemId) : itemId(itemId) {}
		item::Type GetItemSubtype() const;
		void SetQuantity(int) const;
		int GetQuantity() const;
		std::optional<int> TryGetCharacterId() const;
		int GetCharacterId() const;
		void SetCharacterId(int) const;
	private:
		int itemId;
	};
}