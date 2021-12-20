#pragma once
#include "Game.Session.Item.Type.Commodities.h"
#include <string>
namespace game::session::item
{
	struct Type
	{
		constexpr Type(int itemSubtypeId): itemSubtypeId(itemSubtypeId) {}
		constexpr explicit operator int() const { return itemSubtypeId; }
		constexpr item::type::Commodities GetCommodities() const { return item::type::Commodities(itemSubtypeId); }
		constexpr bool operator==(const Type& other) const { return itemSubtypeId == other.itemSubtypeId; }
		std::string GetName() const;
	private:
		int itemSubtypeId;
	};
}