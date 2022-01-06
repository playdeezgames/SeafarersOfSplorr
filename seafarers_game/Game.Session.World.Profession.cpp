#include "Game.Session.World.Profession.h"
#include <map>
namespace game::session::world
{
	struct ProfessionDescriptor
	{
		std::string name;
	};

	static const std::map<game::Profession, ProfessionDescriptor> professionDescriptors =
	{
		{game::Profession::HUNTER, { "Hunter" }},
		{game::Profession::SAILOR, { "Sailor" }},
		{game::Profession::THIEF, { "Thief" }},
		{game::Profession::WARRIOR, { "Warrior" }}
	};

	static const ProfessionDescriptor& GetDescriptor(const game::Profession& profession)
	{
		return professionDescriptors.find(profession)->second;
	}

	std::string Profession::GetName() const
	{
		return GetDescriptor(profession).name;
	}
}