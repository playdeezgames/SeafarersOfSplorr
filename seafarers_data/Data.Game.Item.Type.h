#pragma once
#include <optional>
#include <string>
namespace data::game::item
{
	struct Type
	{
		Type() = delete;
		static void Initialize();
		static int ReadNextTypeForCategory(int category);
		static int EstablishTypeForCategory(int category, int type, const std::string& name);
		static std::optional<int> ReadCategory(int itemTypeId);
		static std::optional<int> ReadType(int itemTypeId);
		static std::optional<std::string> ReadName(int itemTypeId);
		static void Clear();
	};
}
