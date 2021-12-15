#pragma once
#include <optional>
namespace data::game
{
	struct ItemSubtype
	{
		static int ReadNextSubtype(int);
		static int Establish(int, int);
		static std::optional<int> ReadItemType(int);
		static std::optional<int> ReadSubtype(int);
	};
}
