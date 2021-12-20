#pragma once
#include "Game.Session.Item.Type.h"
#include <optional>
namespace game::session
{
	struct Item
	{
		constexpr Item(int itemInstanceId) : itemInstanceId(itemInstanceId) {}
		ItemSubtype GetItemSubtype() const;
		void SetQuantity(int) const;
		int GetQuantity() const;
		std::optional<int> TryGetCharacterId() const;
		int GetCharacterId() const;
		void SetCharacterId(int characterId) const;
	private:
		int itemInstanceId;
	};
}