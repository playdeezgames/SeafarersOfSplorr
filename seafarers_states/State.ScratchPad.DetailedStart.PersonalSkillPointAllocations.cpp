#include <Game.Session.Player.h>
#include "State.ScratchPad.DetailedStart.PersonalSkillPointAllocations.h"
#include <numeric>
#include <Game.Session.Character.Characteristics.h>
namespace state::scratch_pad::detailed_start
{
	std::map<int, size_t> PersonalSkillPointAllocations::personalSkillPointAllocations;

	void PersonalSkillPointAllocations::Clear()
	{
		personalSkillPointAllocations.clear();
	}

	size_t PersonalSkillPointAllocations::GetSkillPointsAllocated()
	{
		return std::accumulate(
			personalSkillPointAllocations.begin(),
			personalSkillPointAllocations.end(),
			(size_t)0,
			[](size_t accumulator, const std::pair<int, size_t> entry)
			{
				return accumulator + entry.second;
			});
	}

	size_t PersonalSkillPointAllocations::GetSkillPointCount()
	{
		const size_t INT_MULTIPLIER = 10;
		size_t intelligence =
			(size_t)game::session::character::Characteristics(game::session::Player::GetCharacterId())
			.GetCharacteristic(game::Characteristic::INTELLIGENCE)
			.GetValue();
		return intelligence * INT_MULTIPLIER;
	}

	size_t PersonalSkillPointAllocations::GetSkillPointsRemaining()
	{
		return GetSkillPointCount() - GetSkillPointsAllocated();
	}

	const std::map<int, size_t>& PersonalSkillPointAllocations::GetSkillPointAllocations()
	{
		return personalSkillPointAllocations;
	}

	void PersonalSkillPointAllocations::AllocateSkillPoints(int skillId, size_t points)
	{
		personalSkillPointAllocations[skillId] += points;
	}
}