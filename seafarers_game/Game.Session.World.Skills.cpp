#include <algorithm>
#include <Data.Game.Skill.h>
#include <Data.Game.SkillCharacteristicCoefficient.h>
#include "Game.Characteristic.h"
#include "Game.Session.World.Skills.h"
#include "Game.SkillCategory.h"
#include <map>
namespace game::session::world
{
	using SkillData = data::game::Skill;

	struct SkillDescriptor
	{
		std::string name;
		int defaultValue;
		std::map<game::Characteristic, int> characteristicCoefficients;
	};

	static const std::map<game::SkillCategory, SkillDescriptor> generalSkillCategories =
	{
		{game::SkillCategory::APPRAISE,{"Appraise", 15, {}}},
		{game::SkillCategory::BARGAIN,{"Bargain", 5, {}}},
		{game::SkillCategory::BRAWL,{"Brawl", 25, {}}},
		{game::SkillCategory::CLIMB,{"Climb", 40, {}}},
		{game::SkillCategory::COMMAND,{"Command", 5, {}}},
		{game::SkillCategory::DEMOLITION,{"Demolition", 1, {}}},
		{game::SkillCategory::DISGUISE,{"Disguise", 1, {}}},
		{game::SkillCategory::DODGE,{"Dodge", 0, {{game::Characteristic::DEXTERITY, 2}}}},
		{game::SkillCategory::ETIQUETTE,{"Etiquette", 5, {}}},
		{game::SkillCategory::FAST_TALK,{"Fast Talk", 5, {}}},
		{game::SkillCategory::FINE_MANIPULATION,{"Fine Manipulation", 5, {}}},
		{game::SkillCategory::FIRST_AID,{"First Aid", 0, {{game::Characteristic::INTELLIGENCE, 1}}}},
		{game::SkillCategory::GAMING,{"Gaming", 0, {{game::Characteristic::INTELLIGENCE,1},{game::Characteristic::POWER,1}}}},
		{game::SkillCategory::GRAPPLE,{"Grapple", 25, {}}},
		{game::SkillCategory::HIDE,{"Hide", 10, {}}},
		{game::SkillCategory::INSIGHT,{"Insight", 5, {}}},
		{game::SkillCategory::JUMP,{"Jump", 25, {}}},
		{game::SkillCategory::LISTEN,{"Listen", 25, {}}},
		{game::SkillCategory::MARTIAL_ARTS,{"Martial Arts", 1, {}}},
		{game::SkillCategory::MEDICINE,{"Medicine", 0, {}}},
		{game::SkillCategory::NAVIGATE,{"Navigate", 10, {}}},
		{game::SkillCategory::PERSUADE,{"Persuade", 15, {}}},
		{game::SkillCategory::RESEARCH,{"Research", 25, {}}},
		{game::SkillCategory::SENSE,{"Sense", 10, {}}},
		{game::SkillCategory::SLIGHT_OF_HAND,{"Slight of Hand", 5, {}}},
		{game::SkillCategory::SPOT,{"Spot", 25, {}}},
		{game::SkillCategory::STEALTH,{"Stealth", 10, {}}},
		{game::SkillCategory::STRATEGY,{"Strategy", 1, {}}},
		{game::SkillCategory::SWIM,{"Swim", 25, {}}},
		{game::SkillCategory::TEACH,{"Teach", 10, {}}},
		{game::SkillCategory::THROW,{"Throw", 25, {}}},
		{game::SkillCategory::TRACK,{"Track", 10, {}}}
	};

	void Skills::Reset() const
	{
		SkillData::Clear();
	}

	void Skills::Populate(const Difficulty&) const
	{
		std::for_each(
			generalSkillCategories.begin(),
			generalSkillCategories.end(),
			[](const std::pair<game::SkillCategory, SkillDescriptor>& entry) 
			{
				auto descriptor = entry.second;
				auto skillId = 
					data::game::Skill::EstablishGeneralSkillForCategory(
					(int)entry.first,
					descriptor.name, 
					descriptor.defaultValue);
				std::for_each(
					descriptor.characteristicCoefficients.begin(),
					descriptor.characteristicCoefficients.end(),
					[skillId](const std::pair<game::Characteristic, int> coefficient) 
					{
						data::game::SkillCharacteristicCoefficient::Write(skillId, (int)coefficient.first, coefficient.second);
					});
			});
	}
}