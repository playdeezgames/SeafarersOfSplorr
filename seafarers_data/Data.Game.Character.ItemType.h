#pragma once
#include <optional>
#include <vector>
namespace data::game::character
{
	struct ItemType
	{
		ItemType() = delete;
		static void Initialize();
		static void Write(int characterId, int itemTypeId, int quantity);
		static void Clear(int characterId, int itemTypeId);
		static std::optional<int> Read(int characterId, int itemTypeId);
		static std::vector<int> ReadForCharacter(int characterId);
		static std::optional<int> ReadCountForCharacter(int characterId);
	};
}