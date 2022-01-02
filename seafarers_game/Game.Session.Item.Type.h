#pragma once
#include "Game.Session.Item.Type.Commodities.h"
#include <string>
namespace game::session::item
{
	struct Type
	{
		constexpr explicit Type(int typeId): typeId(typeId) {}
		constexpr explicit operator int() const { return typeId; }
		constexpr item::type::Commodities GetCommodities() const { return item::type::Commodities(typeId); }
		constexpr bool operator==(const Type& other) const { return typeId == other.typeId; }
		std::string GetName() const;
	private:
		int typeId;
	};
}