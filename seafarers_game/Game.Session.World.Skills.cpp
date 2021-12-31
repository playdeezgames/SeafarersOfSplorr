#include <algorithm>
#include <Data.Game.Skill.h>
#include "Game.Session.World.Skills.h"
#include "Game.SkillCategory.h"
#include <map>
namespace game::session::world
{
	using SkillData = data::game::Skill;

	static const std::map<game::SkillCategory, std::string> generalSkillCategories =
	{
		{game::SkillCategory::APPRAISE,"Appraise"},
		{game::SkillCategory::BARGAIN,"Bargain"},
		{game::SkillCategory::BRAWL,"Brawl"},
		{game::SkillCategory::CLIMB,"Climb"},
		{game::SkillCategory::COMMAND,"Command"},
		{game::SkillCategory::DEMOLITION,"Demolition"},
		{game::SkillCategory::DISGUISE,"Disguise"},
		{game::SkillCategory::DODGE,"Dodge"},
		{game::SkillCategory::ETIQUETTE,"Etiquette"},
		{game::SkillCategory::FAST_TALK,"Fast Talk"},
		{game::SkillCategory::FINE_MANIPULATION,"Fine Manipulation"},
		{game::SkillCategory::FIRST_AID,"First Aid"},
		{game::SkillCategory::GAMING,"Gaming"},
		{game::SkillCategory::GRAPPLE,"Grapple"},
		{game::SkillCategory::HIDE,"Hide"},
		{game::SkillCategory::INSIGHT,"Insight"},
		{game::SkillCategory::JUMP,"Jump"},
		{game::SkillCategory::LISTEN,"Listen"},
		{game::SkillCategory::MARTIAL_ARTS,"Martial Arts"},
		{game::SkillCategory::MEDICINE,"Medicine"},
		{game::SkillCategory::NAVIGATE,"Navigate"},
		{game::SkillCategory::PERSUADE,"Persuade"},
		{game::SkillCategory::RESEARCH,"Research"},
		{game::SkillCategory::SENSE,"Sense"},
		{game::SkillCategory::SLIGHT_OF_HAND,"Slight of Hand"},
		{game::SkillCategory::SPOT,"Spot"},
		{game::SkillCategory::STEALTH,"Stealth"},
		{game::SkillCategory::STRATEGY,"Strategy"},
		{game::SkillCategory::SWIM,"Swim"},
		{game::SkillCategory::TEACH,"Teach"},
		{game::SkillCategory::THROW,"Throw"},
		{game::SkillCategory::TRACK,"Track"}
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
			[](const std::pair<game::SkillCategory, std::string>& entry) 
			{
				data::game::Skill::EstablishGeneralSkillForCategory((int)entry.first,entry.second);
			});
	}
}