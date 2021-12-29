#pragma once
#include <list>
#include <optional>
namespace data::game::character
{
	struct Island
	{
		typedef int characterid_t;
		typedef int islandid_t;
		static std::optional<islandid_t> Read(characterid_t);
		static void Write(characterid_t, islandid_t);
		static void Clear(characterid_t);
		static std::list<characterid_t> All(islandid_t);
	};
}
