#pragma once
#include <map>
#include <optional>
namespace data::game::avatar
{
	struct Equipment
	{
		static std::optional<int> Read(int);
		static void Write(int, const std::optional<int>&);
		static void Clear();
		static std::map<int, int> All();
	};
}