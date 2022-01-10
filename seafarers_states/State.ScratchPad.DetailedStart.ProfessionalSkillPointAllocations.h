#pragma once
#include <map>
namespace state::scratch_pad::detailed_start
{
	struct ProfessionalSkillPointAllocations
	{
		ProfessionalSkillPointAllocations() = delete;
		static const size_t SKILL_POINT_COUNT = 300;
		static void Clear();
		static const std::map<int, size_t>& GetAllocations();
		static size_t GetSkillPointsAllocated();
		static size_t GetSkillPointsRemaining();
		static void AllocateSkillPoints(int skillId, size_t points);
	private:
		static std::map<int, size_t> professionalSkillPointAllocations;
	};
}