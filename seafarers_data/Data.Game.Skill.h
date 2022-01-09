#pragma once
#include <optional>
#include <string>
#include <vector>
namespace data::game
{
	struct Skill
	{
		Skill() = delete;
		static void Initialize();
		static int ReadNextTypeForCategory(int category);
		static int EstablishTypeForCategory(int category, int type, const std::string& name, int defaultValue);
		static int EstablishGeneralSkillForCategory(int category, const std::string& name, int defaultValue);
		static std::optional<int> ReadGeneralSkillForCategory(int category);
		static std::optional<int> ReadCategory(int skillId);
		static std::optional<int> ReadType(int skillId);
		static std::optional<std::string> ReadName(int skillId);
		static std::optional<int> ReadDefaultValue(int skillId);
		static std::vector<int> ReadSkillsForCategory(int category);
		static std::vector<int> All();
		static void Clear();
	};
}
