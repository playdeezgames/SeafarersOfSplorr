#pragma once
#include "Game.Difficulty.h"
#include "Game.SkillCategory.h"
#include "Game.Profession.h"
#include "Game.Session.Character.h"
#include "Game.Session.Skill.h"
#include "Game.Session.SkillCategory.h"
#include <map>
#include <optional>
#include <set>
namespace game::session
{
	struct Player
	{
		constexpr Player() {}
		void Reset() const;
		void Populate(const Difficulty& difficulty, const std::map<int, size_t> allocations) const;
		Character GetCharacter() const;
		std::optional<Character> TryGetCharacter() const;

		static const std::set<Skill>& GetProfessionalSkillSet();

		static void GenerateProfessionalSkillList(const game::Profession& profession, const std::set<SkillCategory>& categorySet);

		static void ClearPersonalSkillAllocations();
		static size_t GetPersonalSkillPointsAllocated();
		static size_t GetPersonalSkillPointCount();
		static size_t GetPersonalSkillPointsRemaining();
		static const std::map<int, size_t>& GetPersonalSkillPointAllocations();
		static void AllocatePersonalSkillPoints(int skillId, size_t points);
		static void DistributePersonalSkillPointAllocations();
	};
}
