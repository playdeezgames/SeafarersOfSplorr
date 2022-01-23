#include "Game.Session.World.Profession.h"
#include <map>
namespace game::session::world
{
	struct ProfessionDescriptor
	{
		std::string name;
		std::set<SkillCategory> categories;
		std::set<SkillCategory> electiveCategories;
	};

	static const std::map<game::Profession, ProfessionDescriptor> professionDescriptors =
	{
		{
			game::Profession::HUNTER, 
			{ 
				"Hunter" ,
				{
					game::SkillCategory::CLIMB,
					game::SkillCategory::HIDE,
					game::SkillCategory::LISTEN,
					game::SkillCategory::NAVIGATE,
					game::SkillCategory::SPOT,
					game::SkillCategory::STEALTH,
					game::SkillCategory::TRACK
				},
				{
					(game::SkillCategory::FIREARM),
					(game::SkillCategory::KNOWLEDGE),
					(game::SkillCategory::MELEE_WEAPON),
					(game::SkillCategory::MISSILE_WEAPON),
					(game::SkillCategory::LANGUAGE),
					(game::SkillCategory::RIDE)
				}
			},
		},
		{
			game::Profession::SAILOR, 
			{ 
				"Sailor",
				{
					(game::SkillCategory::CLIMB),
					(game::SkillCategory::CRAFT),
					(game::SkillCategory::DODGE),
					(game::SkillCategory::GRAPPLE),
					(game::SkillCategory::NAVIGATE),
					(game::SkillCategory::PILOT),
					(game::SkillCategory::SWIM)
				},
				{
					(game::SkillCategory::ARTILLERY),
					(game::SkillCategory::COMMAND),
					(game::SkillCategory::LANGUAGE),
					(game::SkillCategory::LISTEN),
					(game::SkillCategory::REPAIR),
					(game::SkillCategory::SPOT)
				}
			},
		},
		{	game::Profession::THIEF, 
			{ 
				"Thief",
				{
					(game::SkillCategory::APPRAISE),
					(game::SkillCategory::DODGE),
					(game::SkillCategory::FAST_TALK),
					(game::SkillCategory::HIDE),
					(game::SkillCategory::STEALTH)
				},
				{
					(game::SkillCategory::BARGAIN),
					(game::SkillCategory::BRAWL),
					(game::SkillCategory::CLIMB),
					(game::SkillCategory::DISGUISE),
					(game::SkillCategory::FINE_MANIPULATION),
					(game::SkillCategory::FIREARM),
					(game::SkillCategory::GRAPPLE),
					(game::SkillCategory::INSIGHT),
					(game::SkillCategory::LISTEN),
					(game::SkillCategory::JUMP),
					(game::SkillCategory::KNOWLEDGE),
					(game::SkillCategory::PERSUADE),
					(game::SkillCategory::REPAIR),
					(game::SkillCategory::SPOT)
				}
			},
		},
		{
			game::Profession::WARRIOR, 
			{ 
				"Warrior",
				{
					(game::SkillCategory::BRAWL),
					(game::SkillCategory::DODGE),
					(game::SkillCategory::GRAPPLE),
					(game::SkillCategory::MELEE_WEAPON),
					(game::SkillCategory::MISSILE_WEAPON)
				},
				{
					(game::SkillCategory::CLIMB),
					(game::SkillCategory::FIREARM),
					(game::SkillCategory::HIDE),
					(game::SkillCategory::LISTEN),
					(game::SkillCategory::JUMP),
					(game::SkillCategory::LANGUAGE),
					(game::SkillCategory::MARTIAL_ARTS),
					(game::SkillCategory::RIDE),
					(game::SkillCategory::SPOT),
					(game::SkillCategory::STEALTH),
					(game::SkillCategory::SWIM),
					(game::SkillCategory::THROW),
					(game::SkillCategory::TRACK)
				}
			}
		},
		{
			game::Profession::CUSTOM,
			{
				"Custom",
				{
				},
				{
					(game::SkillCategory::APPRAISE),
					(game::SkillCategory::ART),
					(game::SkillCategory::ARTILLERY),
					(game::SkillCategory::BARGAIN),
					(game::SkillCategory::BRAWL),
					(game::SkillCategory::CLIMB),
					(game::SkillCategory::COMMAND),
					(game::SkillCategory::CRAFT),
					(game::SkillCategory::DEMOLITION),
					(game::SkillCategory::DISGUISE),
					(game::SkillCategory::DODGE),
					(game::SkillCategory::ETIQUETTE),
					(game::SkillCategory::FAST_TALK),
					(game::SkillCategory::FINE_MANIPULATION),
					(game::SkillCategory::FIREARM),
					(game::SkillCategory::FIRST_AID),
					(game::SkillCategory::GAMING),
					(game::SkillCategory::GRAPPLE),
					(game::SkillCategory::HEAVY_MACHINE),
					(game::SkillCategory::HIDE),
					(game::SkillCategory::INSIGHT),
					(game::SkillCategory::JUMP),
					(game::SkillCategory::KNOWLEDGE),
					(game::SkillCategory::LANGUAGE),
					(game::SkillCategory::LISTEN),
					(game::SkillCategory::LITERACY),
					(game::SkillCategory::MARTIAL_ARTS),
					(game::SkillCategory::MEDICINE),
					(game::SkillCategory::MELEE_WEAPON),
					(game::SkillCategory::MISSILE_WEAPON),
					(game::SkillCategory::NAVIGATE),
					(game::SkillCategory::PERFORM),
					(game::SkillCategory::PERSUADE),
					(game::SkillCategory::PILOT),
					(game::SkillCategory::PSYCHOTHERAPY),
					(game::SkillCategory::REPAIR),
					(game::SkillCategory::RESEARCH),
					(game::SkillCategory::RIDE),
					(game::SkillCategory::SENSE),
					(game::SkillCategory::SHIELD),
					(game::SkillCategory::SLIGHT_OF_HAND),
					(game::SkillCategory::SPOT),
					(game::SkillCategory::STATUS),
					(game::SkillCategory::STEALTH),
					(game::SkillCategory::STRATEGY),
					(game::SkillCategory::SWIM),
					(game::SkillCategory::TEACH),
					(game::SkillCategory::TECHNICAL_SKILL),
					(game::SkillCategory::THROW),
					(game::SkillCategory::TRACK)
				}
			}
		}
	};

	static const ProfessionDescriptor& GetDescriptor(const game::Profession& profession)
	{
		return professionDescriptors.find(profession)->second;
	}

	const std::string& Profession::GetName() const
	{
		return GetDescriptor(profession).name;
	}

	const std::set<game::SkillCategory>& Profession::GetSkillCategories() const
	{
		return GetDescriptor(profession).categories;
	}

	const std::set<game::SkillCategory>& Profession::GetElectiveSkillCategories() const
	{
		return GetDescriptor(profession).electiveCategories;
	}

}