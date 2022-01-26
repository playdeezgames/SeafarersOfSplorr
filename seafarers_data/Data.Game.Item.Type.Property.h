#pragma once
#include <string>
#include <optional>
namespace data::game::item::type
{
	struct Property
	{
		Property() = default;
		static void InitializeInt();
		static void WriteInt(int itemTypeId, const std::string& key, int value);
		static std::optional<int> ReadInt(int itemTypeId, const std::string& key);
	};
}
