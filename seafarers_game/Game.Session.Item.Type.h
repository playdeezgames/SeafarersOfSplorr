#pragma once
#include <string>
namespace game::session::item
{
	struct Type
	{
		constexpr explicit Type(int typeId): typeId(typeId) {}
		constexpr explicit operator int() const { return typeId; }
		constexpr bool operator==(const Type& other) const { return typeId == other.typeId; }
		std::string GetName() const;
	private:
		int typeId;
	};
}