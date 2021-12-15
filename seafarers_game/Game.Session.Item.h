#pragma once
#include "Game.Session.ItemType.h"
#include <optional>
namespace game::session
{
	struct Item
	{
		Item(int);
		ItemType GetItemType() const;
		void SetQuantity(int) const;
		int GetQuantity() const;
		std::optional<int> TryGetCharacterId() const;
		int GetCharacterId() const;
		void SetCharacterId(int characterId) const;
	private:
		int itemInstanceId;
	};
}