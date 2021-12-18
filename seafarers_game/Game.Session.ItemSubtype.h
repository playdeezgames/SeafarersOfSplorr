#pragma once
namespace game::session
{
	struct ItemSubtype
	{
		constexpr ItemSubtype(int itemSubtypeId): itemSubtypeId(itemSubtypeId) {}
		constexpr explicit operator int() const { return itemSubtypeId; }
	private:
		int itemSubtypeId;
	};
}