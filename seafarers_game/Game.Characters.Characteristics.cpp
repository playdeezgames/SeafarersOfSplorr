#include <Common.RNG.h>
#include <Data.Game.Character.Characteristic.h>
#include "Game.Characters.Characteristics.h"
#include <map>
namespace game::characters
{
	std::optional<int> Characteristics::GetMaximumHitPoints(int characterId)
	{
		auto constitution = data::game::character::Characteristic::Read(characterId, (int)Characteristic::CONSTITUTION);
		if (constitution)
		{
			auto size = data::game::character::Characteristic::Read(characterId, (int)Characteristic::SIZE);
			if (size)
			{
				return (constitution.value() + size.value() + 1) / 2;//CON+SIZ/2, round up!
			}
		}
		return std::nullopt;
	}

	std::optional<bool> Characteristics::Check(int characterId, const Characteristic& characteristic)
	{
		return OpposedCheck(characterId, characteristic, 10);
	}

	std::optional<bool> Characteristics::OpposedCheck(int characterId, const Characteristic& characteristic, int opposition)
	{
		auto value = data::game::character::Characteristic::Read(characterId, (int)characteristic);
		if (value)
		{
			return common::RNG::FromRange(1, 21) < (10 + value.value() - opposition);//d20 < (10 + my characteristic - opposed characteristic)
		}
		return std::nullopt;
	}

	void Characteristics::OnCheck(int characterId, const Characteristic& characteristic, std::function<void(bool)> resultFunc)
	{
		OnOpposedCheck(characterId, characteristic, 0, resultFunc);
	}

	void Characteristics::OnOpposedCheck(int characterId, const Characteristic& characteristic, int opposition, std::function<void(bool)> resultFunc)
	{
		auto checkResult = OpposedCheck(characterId, characteristic, opposition);
		if (checkResult)
		{
			resultFunc(checkResult.value());
		}
	}

}