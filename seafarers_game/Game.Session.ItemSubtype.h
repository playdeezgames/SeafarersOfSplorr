#pragma once
namespace game::session
{
	struct ItemSubtype
	{
		constexpr ItemSubtype(int itemSubtypeId): itemSubtypeId(itemSubtypeId) {}
	private:
		int itemSubtypeId;
	};
}