#pragma once
#include <vector>
#include <optional>
namespace data::game::character
{
	struct Item//TODO: eliminate this
	{
		Item() = delete;
		static void Initialize();
		static void Write(int characterId, int itemId);
		static std::vector<int> ReadForCharacter(int characterId);
		static int ReadCountForCharacter(int characterId);
		static std::optional<int> ReadForItemInstance(int itemId);
	};
}