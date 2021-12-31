#pragma once
#include <list>
#include <optional>
namespace data::game::character
{
	struct ItemInstance
	{
		static void Initialize();
		static void Write(int characterId, int itemId);
		static std::list<int> ReadForCharacter(int characterId);
		static std::optional<int> ReadForItemInstance(int itemId);
	};
}