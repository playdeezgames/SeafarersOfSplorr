#pragma once
#include "Game.Difficulty.h"
#include "Game.SkillCategory.h"
#include "Game.Profession.h"
#include "Game.Session.Character.h"
#include "Game.Session.SkillCategory.h"
#include <optional>
#include <set>
namespace game::session
{
	struct Player
	{
		static const size_t SKILL_CATEGORY_COUNT = 10;
		static const size_t PROFESSIONAL_SKILL_POINT_COUNT = 300;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		Character GetCharacter() const;
		std::optional<Character> TryGetCharacter() const;
		//coordinates UI metastate
		static void SetCrewDetailCharacterId(int characterId);
		static int GetCrewDetailCharacterId();
		static void SetRenameShipAdjective(const std::string& adjective);
		static const std::string& GetRenameShipAdjective();
		static void SetHeadForMark(const std::string& markName);
		static const std::string GetHeadForMark();
		static void SetProfession(const game::Profession& profession);
		static game::Profession GetProfession();
		static void ClearElectiveSkillCategories();
		static std::set<game::session::SkillCategory> GetElectiveSkillCategories();
		static void AddElectiveSkillCategory(const game::SkillCategory& category);
		static void SetCharacterCreationDefaults();
		static void ClearProfessionSkillPointAllocations();
	};
}
