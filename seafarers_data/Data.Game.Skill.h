#pragma once
#include <optional>
#include <string>
namespace data::game
{
	struct Skill
	{
		Skill() = delete;
		static void Initialize();
		static int ReadNextTypeForCategory(int category);
		static int EstablishTypeForCategory(int category, int type, const std::string& name);
		static std::optional<int> ReadCategory(int skillId);
		static std::optional<int> ReadType(int skillId);
		static std::optional<std::string> ReadName(int skillId);
		static void Clear();
	};
}
