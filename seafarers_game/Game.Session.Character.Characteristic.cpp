#include "Game.Characteristics.h"
#include "Game.Characters.Characteristics.h"
#include "Game.Session.Character.Characteristic.h"
namespace game::session::character
{
	Characteristic::Characteristic(int characterId, const game::Characteristic& characteristic)
		: characterId(characterId)
		, characteristic(characteristic)
	{

	}

	const std::string& Characteristic::GetName() const
	{
		return game::Characteristics::GetName(characteristic);
	}

	static std::optional<int> TryGetValue(int characterId, const game::Characteristic& characteristic)
	{
		return game::characters::Characteristics::Read(characterId, characteristic);
	}

	int Characteristic::GetValue() const
	{
		return TryGetValue(characterId, characteristic).value();
	}

}