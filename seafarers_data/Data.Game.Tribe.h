#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Tribe
	{
		static void Initialize();
		static int Create(const std::string& name);
		static std::list<int> All();
		static std::optional<std::string> ReadName(int);
		static void Clear();
	};
}
