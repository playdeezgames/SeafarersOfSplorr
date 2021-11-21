#pragma once
#include <map>
#include <optional>
namespace data::game::character
{
	struct Equipment
	{
		static std::optional<int> Read(int, int);
		static void Write(int, int, const std::optional<int>&);
		static void Clear(int);
		static std::map<int, int> All(int);
	};
}