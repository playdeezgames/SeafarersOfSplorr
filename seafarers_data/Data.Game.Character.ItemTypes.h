#pragma once
#include <optional>
namespace data::game::character
{
	struct ItemTypes
	{
		ItemTypes() = delete;
		static void Initialize();
		static void Write(int characterId, int itemTypeId, int count);
		static std::optional<int> Read(int characterId, int itemTypeId);
	};
}