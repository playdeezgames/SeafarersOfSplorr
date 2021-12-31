#pragma once
#include <set>
namespace data::game::character
{
	struct Flags
	{
		static void Initialize();
		static bool Has(int characterId, int flag);
		static void Write(int characterId, int flag);
		static void Clear(int characterId, int flag);
		static std::set<int> All(int characterId);
	};
}