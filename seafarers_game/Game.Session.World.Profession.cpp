#include "Game.Session.World.Profession.h"
#include <map>
namespace game::session::world
{
	struct ProfessionDescriptor
	{
		std::string name;
		std::vector<SkillCategory> categories;
		std::vector<SkillCategory> electiveCategories;
	};

	static const std::map<game::Profession, ProfessionDescriptor> professionDescriptors =
	{
		{
			game::Profession::HUNTER, 
			{ 
				"Hunter" ,
				{
					SkillCategory(game::SkillCategory::CLIMB),
					SkillCategory(game::SkillCategory::HIDE),
					SkillCategory(game::SkillCategory::LISTEN),
					SkillCategory(game::SkillCategory::NAVIGATE),
					SkillCategory(game::SkillCategory::SPOT),
					SkillCategory(game::SkillCategory::STEALTH),
					SkillCategory(game::SkillCategory::TRACK)
				},
				{
					SkillCategory(game::SkillCategory::FIREARM),
					SkillCategory(game::SkillCategory::KNOWLEDGE),
					SkillCategory(game::SkillCategory::MELEE_WEAPON),
					SkillCategory(game::SkillCategory::MISSILE_WEAPON),
					SkillCategory(game::SkillCategory::LANGUAGE),
					SkillCategory(game::SkillCategory::RIDE)
				}
			},
		},
		{
			game::Profession::SAILOR, 
			{ 
				"Sailor",
				{
					SkillCategory(game::SkillCategory::CLIMB),
					SkillCategory(game::SkillCategory::CRAFT),
					SkillCategory(game::SkillCategory::DODGE),
					SkillCategory(game::SkillCategory::GRAPPLE),
					SkillCategory(game::SkillCategory::NAVIGATE),
					SkillCategory(game::SkillCategory::PILOT),
					SkillCategory(game::SkillCategory::SWIM)
				},
				{
					SkillCategory(game::SkillCategory::ARTILLERY),
					SkillCategory(game::SkillCategory::COMMAND),
					SkillCategory(game::SkillCategory::LANGUAGE),
					SkillCategory(game::SkillCategory::LISTEN),
					SkillCategory(game::SkillCategory::REPAIR),
					SkillCategory(game::SkillCategory::SPOT)
				}
			},
		},
		{	game::Profession::THIEF, 
			{ 
				"Thief",
				{
					SkillCategory(game::SkillCategory::APPRAISE),
					SkillCategory(game::SkillCategory::DODGE),
					SkillCategory(game::SkillCategory::FAST_TALK),
					SkillCategory(game::SkillCategory::HIDE),
					SkillCategory(game::SkillCategory::STEALTH)
				},
				{
					SkillCategory(game::SkillCategory::BARGAIN),
					SkillCategory(game::SkillCategory::BRAWL),
					SkillCategory(game::SkillCategory::CLIMB),
					SkillCategory(game::SkillCategory::DISGUISE),
					SkillCategory(game::SkillCategory::FINE_MANIPULATION),
					SkillCategory(game::SkillCategory::FIREARM),
					SkillCategory(game::SkillCategory::GRAPPLE),
					SkillCategory(game::SkillCategory::INSIGHT),
					SkillCategory(game::SkillCategory::LISTEN),
					SkillCategory(game::SkillCategory::JUMP),
					SkillCategory(game::SkillCategory::KNOWLEDGE),
					SkillCategory(game::SkillCategory::PERSUADE),
					SkillCategory(game::SkillCategory::REPAIR),
					SkillCategory(game::SkillCategory::SPOT)
				}
			},
		},
		{
			game::Profession::WARRIOR, 
			{ 
				"Warrior",
				{
					SkillCategory(game::SkillCategory::BRAWL),
					SkillCategory(game::SkillCategory::DODGE),
					SkillCategory(game::SkillCategory::GRAPPLE),
					SkillCategory(game::SkillCategory::MELEE_WEAPON),
					SkillCategory(game::SkillCategory::MISSILE_WEAPON)
				},
				{
					SkillCategory(game::SkillCategory::CLIMB),
					SkillCategory(game::SkillCategory::FIREARM),
					SkillCategory(game::SkillCategory::HIDE),
					SkillCategory(game::SkillCategory::LISTEN),
					SkillCategory(game::SkillCategory::JUMP),
					SkillCategory(game::SkillCategory::LANGUAGE),
					SkillCategory(game::SkillCategory::MARTIAL_ARTS),
					SkillCategory(game::SkillCategory::RIDE),
					SkillCategory(game::SkillCategory::SPOT),
					SkillCategory(game::SkillCategory::STEALTH),
					SkillCategory(game::SkillCategory::SWIM),
					SkillCategory(game::SkillCategory::THROW),
					SkillCategory(game::SkillCategory::TRACK)
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
					SkillCategory(game::SkillCategory::APPRAISE),
					SkillCategory(game::SkillCategory::ART),
					SkillCategory(game::SkillCategory::ARTILLERY),
					SkillCategory(game::SkillCategory::BARGAIN),
					SkillCategory(game::SkillCategory::BRAWL),
					SkillCategory(game::SkillCategory::CLIMB),
					SkillCategory(game::SkillCategory::COMMAND),
					SkillCategory(game::SkillCategory::CRAFT),
					SkillCategory(game::SkillCategory::DEMOLITION),
					SkillCategory(game::SkillCategory::DISGUISE),
					SkillCategory(game::SkillCategory::DODGE),
					SkillCategory(game::SkillCategory::ETIQUETTE),
					SkillCategory(game::SkillCategory::FAST_TALK),
					SkillCategory(game::SkillCategory::FINE_MANIPULATION),
					SkillCategory(game::SkillCategory::FIREARM),
					SkillCategory(game::SkillCategory::FIRST_AID),
					SkillCategory(game::SkillCategory::GAMING),
					SkillCategory(game::SkillCategory::GRAPPLE),
					SkillCategory(game::SkillCategory::HEAVY_MACHINE),
					SkillCategory(game::SkillCategory::HIDE),
					SkillCategory(game::SkillCategory::INSIGHT),
					SkillCategory(game::SkillCategory::JUMP),
					SkillCategory(game::SkillCategory::KNOWLEDGE),
					SkillCategory(game::SkillCategory::LANGUAGE),
					SkillCategory(game::SkillCategory::LISTEN),
					SkillCategory(game::SkillCategory::LITERACY),
					SkillCategory(game::SkillCategory::MARTIAL_ARTS),
					SkillCategory(game::SkillCategory::MEDICINE),
					SkillCategory(game::SkillCategory::MELEE_WEAPON),
					SkillCategory(game::SkillCategory::MISSILE_WEAPON),
					SkillCategory(game::SkillCategory::NAVIGATE),
					SkillCategory(game::SkillCategory::PERFORM),
					SkillCategory(game::SkillCategory::PERSUADE),
					SkillCategory(game::SkillCategory::PILOT),
					SkillCategory(game::SkillCategory::PSYCHOTHERAPY),
					SkillCategory(game::SkillCategory::REPAIR),
					SkillCategory(game::SkillCategory::RESEARCH),
					SkillCategory(game::SkillCategory::RIDE),
					SkillCategory(game::SkillCategory::SENSE),
					SkillCategory(game::SkillCategory::SHIELD),
					SkillCategory(game::SkillCategory::SLIGHT_OF_HAND),
					SkillCategory(game::SkillCategory::SPOT),
					SkillCategory(game::SkillCategory::STATUS),
					SkillCategory(game::SkillCategory::STEALTH),
					SkillCategory(game::SkillCategory::STRATEGY),
					SkillCategory(game::SkillCategory::SWIM),
					SkillCategory(game::SkillCategory::TEACH),
					SkillCategory(game::SkillCategory::TECHNICAL_SKILL),
					SkillCategory(game::SkillCategory::THROW),
					SkillCategory(game::SkillCategory::TRACK)
				}
			}
		}
	};

	static const ProfessionDescriptor& GetDescriptor(const game::Profession& profession)
	{
		return professionDescriptors.find(profession)->second;
	}

	std::string Profession::GetName() const
	{
		return GetDescriptor(profession).name;
	}

	const std::vector<SkillCategory>& Profession::GetSkillCategories() const
	{
		return GetDescriptor(profession).categories;
	}

	const std::vector<SkillCategory>& Profession::GetElectiveSkillCategories() const
	{
		return GetDescriptor(profession).electiveCategories;
	}

}