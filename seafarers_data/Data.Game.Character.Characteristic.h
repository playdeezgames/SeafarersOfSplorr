#pragma once
#include <optional>
#include <map>
namespace data::game::character
{
	struct Characteristic
	{
		static void Write(int, int, int);
		static std::optional<int> Read(int, int);
		static std::map<int, int> Read(int);
	};
}
