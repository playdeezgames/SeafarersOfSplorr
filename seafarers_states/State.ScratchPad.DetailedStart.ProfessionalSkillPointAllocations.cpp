#include <numeric>
#include "State.ScratchPad.DetailedStart.ProfessionalSkillPointAllocations.h"
namespace state::scratch_pad::detailed_start
{
	std::map<int, size_t> ProfessionalSkillPointAllocations::professionalSkillPointAllocations;

	void ProfessionalSkillPointAllocations::Clear()
	{
		professionalSkillPointAllocations.clear();
	}

	const std::map<int, size_t>& ProfessionalSkillPointAllocations::GetAllocations()
	{
		return professionalSkillPointAllocations;
	}

	size_t ProfessionalSkillPointAllocations::GetSkillPointsAllocated()
	{
		return std::accumulate(
			professionalSkillPointAllocations.begin(),
			professionalSkillPointAllocations.end(),
			(size_t)0,
			[](size_t accumulator, const std::pair<int, size_t> entry)
			{
				return accumulator + entry.second;
			});
	}

	size_t ProfessionalSkillPointAllocations::GetSkillPointsRemaining()
	{
		return SKILL_POINT_COUNT - GetSkillPointsAllocated();
	}

	void ProfessionalSkillPointAllocations::AllocateSkillPoints(int skillId, size_t points)
	{
		professionalSkillPointAllocations[skillId] += points;
	}

}