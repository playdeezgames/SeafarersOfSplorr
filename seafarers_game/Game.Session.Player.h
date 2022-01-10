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
		void Populate(const Difficulty& difficulty) const;
		Character GetCharacter() const;
		std::optional<Character> TryGetCharacter() const;

		static const std::set<Skill>& GetProfessionalSkillSet();

		static void SetCharacterCreationDefaults();

		static const size_t PROFESSIONAL_SKILL_POINT_COUNT = 300;
		static void ClearProfessionSkillPointAllocations();
		static const std::map<int, size_t>& GetProfessionSkillPointAllocations();
		static size_t GetProfessionalSkillPointsAllocated();
		static size_t GetProfessionalSkillPointsRemaining();
		static void AllocateProfessionalSkillPoints(int skillId, size_t points);
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
