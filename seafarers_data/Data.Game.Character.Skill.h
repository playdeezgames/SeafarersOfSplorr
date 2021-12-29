#pragma once
#include <map>
#include <optional>
namespace data::game::character
{
	struct Skill
	{
		static void Initialize();
		static void Write(int, int, int);
		static std::optional<int> Read(int, int);
		static std::map<int, int> Read(int);
	};
}
