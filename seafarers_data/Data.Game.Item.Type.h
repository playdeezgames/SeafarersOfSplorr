#pragma once
#include <optional>
#include <string>
namespace data::game::item
{
	struct Type
	{
		static int ReadNextSubtype(int);
		static int Establish(int, int, const std::string&);
		static std::optional<int> ReadItemType(int);
		static std::optional<int> ReadSubtype(int);
		static std::optional<std::string> ReadName(int);
		static void Clear();
	};
}
