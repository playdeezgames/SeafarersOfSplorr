#pragma once
#include "Game.Item.Category.h"
#include <string>
namespace game::session::item
{
	struct Type
	{
		constexpr explicit Type(int typeId): typeId(typeId) {}
		constexpr explicit operator int() const { return typeId; }
		constexpr bool operator==(const Type& other) const { return typeId == other.typeId; }
		std::string GetName() const;
		bool HasCategory(const game::item::Category& category) const;
	private:
		int typeId;
	};
}