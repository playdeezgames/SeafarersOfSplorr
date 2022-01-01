#include <algorithm>
#include <Data.Game.Character.Characteristic.h>
#include "Game.Session.Character.Characteristics.h"
#include <iterator>
namespace game::session::character
{
	std::list<Characteristic> Characteristics::GetAll() const
	{
		auto characteristics = data::game::character::Characteristic::Read(characterId);
		std::list<Characteristic> result;
		std::transform(
			characteristics.begin(),
			characteristics.end(),
			std::back_inserter(result),
			[this](const std::pair<int, int>& entry) 
			{
				return Characteristic(characterId,(game::Characteristic)entry.first);
			});
		return result;
	}
}