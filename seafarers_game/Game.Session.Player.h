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

		//TODO: these dont make sense here - they goes somewhere in states
		static void SetCrewDetailCharacterId(int characterId);
		static int GetCrewDetailCharacterId();

		static void SetRenameShipAdjective(const std::string& adjective);
		static const std::string& GetRenameShipAdjective();

		static void SetHeadForMark(const std::string& markName);
		static const std::string GetHeadForMark();

		static void SetProfession(const game::Profession& profession);
		static game::Profession GetProfession();

		static const size_t SKILL_CATEGORY_COUNT = 10;
		static void ClearElectiveSkillCategories();
		static std::set<game::session::SkillCategory> GetElectiveSkillCategories();
		static void AddElectiveSkillCategory(const game::SkillCategory& category);
		static const std::set<Skill>& GetProfessionalSkillSet();

		static void SetCharacterCreationDefaults();

		static const size_t PROFESSIONAL_SKILL_POINT_COUNT = 300;
		static void ClearProfessionSkillPointAllocations();
		static const std::map<int, size_t>& GetProfessionSkillPointAllocations();
		static size_t GetProfessionalSkillPointsAllocated();
		static size_t GetProfessionalSkillPointsRemaining();
		static void AllocateProfessionalSkillPoints(int skillId, size_t points);
		static void GenerateProfessionalSkillList();

		static void SetSelectedSkillId(int skillId);
		static int GetSelectedSkillId();

		static void ClearPersonalSkillAllocations();
		static size_t GetPersonalSkillPointsAllocated();
		static size_t GetPersonalSkillPointCount();
		static size_t GetPersonalSkillPointsRemaining();
		static const std::map<int, size_t>& GetPersonalSkillPointAllocations();
		static void AllocatePersonalSkillPoints(int skillId, size_t points);
		static void DistributePersonalSkillPointAllocations();
	};
}
