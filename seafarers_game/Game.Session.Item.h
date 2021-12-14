#pragma once
#include "Game.Item.h"
#include <optional>
namespace game::session
{
	struct Item
	{
		Item(int);
		game::Item GetItemType() const;
		void SetQuantity(int) const;
		int GetQuantity() const;
		std::optional<int> TryGetCharacterId() const;
		int GetCharacterId() const;
		void SetCharacterId(int characterId) const;
	private:
		int itemInstanceId;
	};
}