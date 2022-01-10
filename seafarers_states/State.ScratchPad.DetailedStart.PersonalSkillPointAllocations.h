#pragma once
#include <map>
namespace state::scratch_pad::detailed_start
{
	struct PersonalSkillPointAllocations
	{
		PersonalSkillPointAllocations() = delete;
		static void Clear();
		static size_t GetSkillPointsAllocated();
		static size_t GetSkillPointCount();
		static size_t GetSkillPointsRemaining();
		static const std::map<int, size_t>& GetSkillPointAllocations();
		static void AllocateSkillPoints(int skillId, size_t points);
	private:
		static std::map<int, size_t> personalSkillPointAllocations;
	};
}