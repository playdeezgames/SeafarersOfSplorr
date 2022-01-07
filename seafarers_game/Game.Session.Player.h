#pragma once
#include "Game.Difficulty.h"
#include "Game.SkillCategory.h"
#include "Game.Profession.h"
#include "Game.Session.Character.h"
#include <optional>
#include <vector>
namespace game::session
{
	struct Player
	{
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
		static std::vector<game::SkillCategory> GetElectiveSkillCategories();
		static void AddElectiveSkillCategory(const game::SkillCategory& category);
		//elective skills
		//list of current electives
		//add elective
		//clear elective choices
		static void SetCharacterCreationDefaults();
	};
}
