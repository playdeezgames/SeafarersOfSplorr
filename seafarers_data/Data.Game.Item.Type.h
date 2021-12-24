#pragma once
#include <optional>
#include <string>
namespace data::game::item
{
	struct Type
	{
		static void Initialize();
		static int ReadNextTypeForCategory(int);
		static int EstablishTypeForCategory(int, int, const std::string&);
		static std::optional<int> ReadCategory(int);
		static std::optional<int> ReadType(int);
		static std::optional<std::string> ReadName(int);
		static void Clear();
	};
}
