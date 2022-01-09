#include <algorithm>
#include <Data.Game.Skill.h>
#include <Data.Game.SkillCharacteristicCoefficient.h>
#include "Game.Characteristic.h"
#include "Game.Session.SkillCategory.h"
#include "Game.Session.World.Skills.h"
#include "Game.SkillCategory.h"
#include <iterator>
#include <map>
namespace game::session::world
{
	using SkillData = data::game::Skill;

	void Skills::Reset() const
	{
		SkillData::Clear();
	}

	std::vector<game::SkillCategory> generalSkillCategories =
	{
		game::SkillCategory::APPRAISE,
		game::SkillCategory::BARGAIN,
		game::SkillCategory::BRAWL,
		game::SkillCategory::CLIMB,
		game::SkillCategory::COMMAND,
		game::SkillCategory::DEMOLITION,
		game::SkillCategory::DISGUISE,
		game::SkillCategory::DODGE,
		game::SkillCategory::ETIQUETTE,
		game::SkillCategory::FAST_TALK,
		game::SkillCategory::FINE_MANIPULATION,
		game::SkillCategory::FIRST_AID,
		game::SkillCategory::GAMING,
		game::SkillCategory::GRAPPLE,
		game::SkillCategory::HIDE,
		game::SkillCategory::INSIGHT,
		game::SkillCategory::JUMP,
		game::SkillCategory::LISTEN,
		game::SkillCategory::MARTIAL_ARTS,
		game::SkillCategory::MEDICINE,
		game::SkillCategory::NAVIGATE,
		game::SkillCategory::PERSUADE,
		game::SkillCategory::RESEARCH,
		game::SkillCategory::SENSE,
		game::SkillCategory::SLIGHT_OF_HAND,
		game::SkillCategory::SPOT,
		game::SkillCategory::STEALTH,
		game::SkillCategory::STRATEGY,
		game::SkillCategory::SWIM,
		game::SkillCategory::TEACH,
		game::SkillCategory::THROW,
		game::SkillCategory::TRACK
	};

	void Skills::Populate(const Difficulty&) const
	{
		std::for_each(
			generalSkillCategories.begin(),
			generalSkillCategories.end(),
			[](const game::SkillCategory& category)
			{
				auto descriptor = game::session::SkillCategory(category);
				auto skillId = 
					data::game::Skill::EstablishGeneralSkillForCategory(
					(int)category,
					descriptor.GetName(), 
					descriptor.GetDefaultValue());
				auto coefficients = descriptor.GetCharacteristics();
				std::for_each(
					coefficients.begin(),
					coefficients.end(),
					[skillId, descriptor](const game::Characteristic& characteristic) 
					{
						data::game::SkillCharacteristicCoefficient::Write(
							skillId, 
							(int)characteristic, 
							descriptor.GetCharacteristicCoefficient(characteristic));
					});
			});
	}

	std::set<Skill> Skills::GetSkillsInCategory(const game::SkillCategory& category) const
	{
		std::set<Skill> result;
		auto skillIds = data::game::Skill::ReadSkillsForCategory((int)category);
		std::transform(
			skillIds.begin(),
			skillIds.end(),
			std::inserter(result, result.end()),
			Skill::ToSkill);
		return result;
	}

	std::set<Skill> Skills::GetSkills() const
	{
		std::set<Skill> result;
		auto skillIds = data::game::Skill::All();
		std::transform(
			skillIds.begin(),
			skillIds.end(),
			std::inserter(result, result.end()),
			Skill::ToSkill);
		return result;
	}

}