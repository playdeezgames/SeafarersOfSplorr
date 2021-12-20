#pragma once
#include <map>
#include <optional>
namespace data::game::character
{
	struct Equipment
	{
		static std::optional<int> Read(int, int);
		static void Write(int, int, const std::optional<int>&);//TODO: make a Write(int,int,int) and Clear(int,int)
		static void Clear(int);
		static void Clear();
		static std::map<int, int> All(int);
	};
}