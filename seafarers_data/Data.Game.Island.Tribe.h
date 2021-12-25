#pragma once
#include <map>
#include <optional>
namespace data::game::island
{
	struct Tribe
	{
		static void Initialize();
		static void Clear();
		static void Write(int, int, int);
		static std::optional<int> Read(int, int);
		static std::map<int, int> All(int);
	};
}
