#include "Game.Session.SkillCategories.h"
namespace game::session
{
	std::vector<SkillCategory> allSkillCategories =
	{
		SkillCategory(game::SkillCategory::APPRAISE),
		SkillCategory(game::SkillCategory::BARGAIN),
		SkillCategory(game::SkillCategory::BRAWL),
		SkillCategory(game::SkillCategory::CLIMB),
		SkillCategory(game::SkillCategory::COMMAND),
		SkillCategory(game::SkillCategory::DEMOLITION),
		SkillCategory(game::SkillCategory::DISGUISE),
		SkillCategory(game::SkillCategory::DODGE),
		SkillCategory(game::SkillCategory::ETIQUETTE),
		SkillCategory(game::SkillCategory::FAST_TALK),
		SkillCategory(game::SkillCategory::FINE_MANIPULATION),
		SkillCategory(game::SkillCategory::FIRST_AID),
		SkillCategory(game::SkillCategory::GAMING),
		SkillCategory(game::SkillCategory::GRAPPLE),
		SkillCategory(game::SkillCategory::HIDE),
		SkillCategory(game::SkillCategory::INSIGHT),
		SkillCategory(game::SkillCategory::JUMP),
		SkillCategory(game::SkillCategory::LISTEN),
		SkillCategory(game::SkillCategory::MARTIAL_ARTS),
		SkillCategory(game::SkillCategory::MEDICINE),
		SkillCategory(game::SkillCategory::NAVIGATE),
		SkillCategory(game::SkillCategory::PERSUADE),
		SkillCategory(game::SkillCategory::RESEARCH),
		SkillCategory(game::SkillCategory::SENSE),
		SkillCategory(game::SkillCategory::SLIGHT_OF_HAND),
		SkillCategory(game::SkillCategory::SPOT),
		SkillCategory(game::SkillCategory::STEALTH),
		SkillCategory(game::SkillCategory::STRATEGY),
		SkillCategory(game::SkillCategory::SWIM),
		SkillCategory(game::SkillCategory::TEACH),
		SkillCategory(game::SkillCategory::THROW),
		SkillCategory(game::SkillCategory::TRACK)
	};

	const std::vector<SkillCategory>& SkillCategories::GetSkillCategories() const
	{
		return allSkillCategories;
	}
}