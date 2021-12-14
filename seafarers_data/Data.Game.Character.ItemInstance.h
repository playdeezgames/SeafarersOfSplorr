#pragma once
#include <list>
#include <optional>
namespace data::game::character
{
	struct ItemInstance
	{
		static void Write(int, int);
		static std::list<int> ReadForCharacter(int);
		static std::optional<int> ReadForItemInstance(int);
		static void Purge();
	};
}