#include "Game.Session.World.Profession.h"
#include <map>
namespace game::session::world
{
	struct ProfessionDescriptor
	{
		std::string name;
		std::vector<game::SkillCategory> categories;
		std::vector<game::SkillCategory> electiveCategories;
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
					game::SkillCategory::FIREARM,
					game::SkillCategory::KNOWLEDGE,
					game::SkillCategory::MELEE_WEAPON,
					game::SkillCategory::MISSILE_WEAPON,
					game::SkillCategory::LANGUAGE,
					game::SkillCategory::RIDE
				}
			},
		},
		{
			game::Profession::SAILOR, 
			{ 
				"Sailor",
				{
					game::SkillCategory::CLIMB,
					game::SkillCategory::CRAFT,
					game::SkillCategory::DODGE,
					game::SkillCategory::GRAPPLE,
					game::SkillCategory::NAVIGATE,
					game::SkillCategory::PILOT,
					game::SkillCategory::SWIM
				},
				{
					game::SkillCategory::ARTILLERY,
					game::SkillCategory::COMMAND,
					game::SkillCategory::LANGUAGE,
					game::SkillCategory::LISTEN,
					game::SkillCategory::REPAIR,
					game::SkillCategory::SPOT
				}
			},
		},
		{	game::Profession::THIEF, 
			{ 
				"Thief",
				{
					game::SkillCategory::APPRAISE,
					game::SkillCategory::DODGE,
					game::SkillCategory::FAST_TALK,
					game::SkillCategory::HIDE,
					game::SkillCategory::STEALTH
				},
				{
					game::SkillCategory::BARGAIN,
					game::SkillCategory::BRAWL,
					game::SkillCategory::CLIMB,
					game::SkillCategory::DISGUISE,
					game::SkillCategory::FINE_MANIPULATION,
					game::SkillCategory::FIREARM,
					game::SkillCategory::GRAPPLE,
					game::SkillCategory::INSIGHT,
					game::SkillCategory::LISTEN,
					game::SkillCategory::JUMP,
					game::SkillCategory::KNOWLEDGE,
					game::SkillCategory::PERSUADE,
					game::SkillCategory::REPAIR,
					game::SkillCategory::SPOT
				}
			},
		},
		{
			game::Profession::WARRIOR, 
			{ 
				"Warrior",
				{
					game::SkillCategory::BRAWL,
					game::SkillCategory::DODGE,
					game::SkillCategory::GRAPPLE,
					game::SkillCategory::MELEE_WEAPON,
					game::SkillCategory::MISSILE_WEAPON
				},
				{
					game::SkillCategory::CLIMB,
					game::SkillCategory::FIREARM,
					game::SkillCategory::HIDE,
					game::SkillCategory::LISTEN,
					game::SkillCategory::JUMP,
					game::SkillCategory::LANGUAGE,
					game::SkillCategory::MARTIAL_ARTS,
					game::SkillCategory::RIDE,
					game::SkillCategory::SPOT,
					game::SkillCategory::STEALTH,
					game::SkillCategory::SWIM,
					game::SkillCategory::THROW,
					game::SkillCategory::TRACK
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

	const std::vector<game::SkillCategory>& Profession::GetSkillCategories() const
	{
		return GetDescriptor(profession).categories;
	}

	const std::vector<game::SkillCategory>& Profession::GetElectiveSkillCategories() const
	{
		return GetDescriptor(profession).electiveCategories;
	}

}