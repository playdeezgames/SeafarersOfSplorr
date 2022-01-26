#pragma once
#include <optional>
#include <string>
namespace data::game::item
{
	struct Type
	{
		Type() = delete;
		static void Initialize();
		static int Create(const std::string& name);
		static std::optional<std::string> ReadName(int itemTypeId);
		static void Clear();
	};
}
