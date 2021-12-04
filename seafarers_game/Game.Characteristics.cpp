#include "Game.Characteristics.h"
#include <map>
namespace game
{
	static const std::map<Characteristic, std::string> characteristicNames =
	{
		{Characteristic::CHARISMA, "Charisma"},
		{Characteristic::CONSTITUTION, "Constitution"},
		{Characteristic::DEXTERITY, "Dexterity"},
		{Characteristic::INTELLIGENCE, "Intelligence"},
		{Characteristic::POWER, "Power"},
		{Characteristic::SIZE, "Size"},
		{Characteristic::STRENGTH, "Strength"}
	};

	const std::string& Characteristics::GetName(const Characteristic& characteristic)
	{
		return characteristicNames.find(characteristic)->second;
	}
}