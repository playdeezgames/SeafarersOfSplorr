#pragma once
#include <map>
#include <optional>
namespace data::game::character
{
	struct Skill
	{
		Skill() = delete;
		static void Initialize();
		static void Write(int characterId, int skillId, int value);
		static std::optional<int> Read(int characterId, int skillId);
		static std::map<int, int> Read(int characterId);
	};
}
