#include <Common.RNG.h>
#include <Data.Game.Character.Characteristic.h>
#include "Game.Characters.Characteristics.h"
#include <map>
namespace game::characters
{
	void Characteristics::Write(int characterId, const Characteristic& characteristic, int value)
	{
		data::game::character::Characteristic::Write(characterId, (int)characteristic, value);
	}

	std::optional<int> Characteristics::Read(int characterId, const Characteristic& characteristic)
	{
		return data::game::character::Characteristic::Read(characterId, (int)characteristic);
	}

	std::map<Characteristic, int> Characteristics::Read(int characterId)
	{
		std::map<Characteristic, int> result;
		auto characteristics = data::game::character::Characteristic::Read(characterId);
		for (auto characteristic : characteristics)
		{
			result[(Characteristic)characteristic.first] = characteristic.second;
		}
		return result;
	}

	static const std::map<int, size_t> _2d6plus6 =
	{
		{8,1},
		{9,2},
		{10,3},
		{11,4},
		{12,5},
		{13,6},
		{14,5},
		{15,4},
		{16,3},
		{17,2},
		{18,1}
	};

	static const std::map<int, size_t> _3d6 =
	{
		{3,1},
		{4,3},
		{5,6},
		{6,10},
		{7,15},
		{8,21},
		{9,25},
		{10,27},
		{11,27},
		{12,25},
		{13,21},
		{14,15},
		{15,10},
		{16,6},
		{17,3},
		{18,1}
	};

	static const std::map<Characteristic, std::map<int, size_t>> characteristicRolls =
	{
		{ Characteristic::CHARISMA, _3d6},
		{ Characteristic::CONSTITUTION, _3d6},
		{ Characteristic::DEXTERITY, _3d6},
		{ Characteristic::INTELLIGENCE, _2d6plus6},
		{ Characteristic::POWER, _3d6},
		{ Characteristic::SIZE, _2d6plus6},
		{ Characteristic::STRENGTH, _3d6},
	};

	void Characteristics::Generate(int characterId)
	{
		for (auto characteristicRoll : characteristicRolls)
		{
			data::game::character::Characteristic::Write(
				characterId, 
				(int)characteristicRoll.first, 
				common::RNG::FromGenerator(characteristicRoll.second));
		}
	}

	std::optional<int> Characteristics::GetMaximumHitPoints(int characterId)
	{
		auto constitution = Read(characterId, Characteristic::CONSTITUTION);
		if (constitution)
		{
			auto size = Read(characterId, Characteristic::SIZE);
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
		auto value = Read(characterId, characteristic);
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