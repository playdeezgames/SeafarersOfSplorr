#include <Common.RNG.h>
#include <Data.Game.Character.Characteristic.h>
#include "Game.Session.Character.Characteristic.h"
namespace game::session::character
{
	static const std::map<game::Characteristic, std::string> characteristicNames =
	{
		{game::Characteristic::CHARISMA, "Charisma"},
		{game::Characteristic::CONSTITUTION, "Constitution"},
		{game::Characteristic::DEXTERITY, "Dexterity"},
		{game::Characteristic::INTELLIGENCE, "Intelligence"},
		{game::Characteristic::POWER, "Power"},
		{game::Characteristic::SIZE, "Size"},
		{game::Characteristic::STRENGTH, "Strength"}
	};

	const std::string& Characteristic::GetName() const
	{
		return characteristicNames.find(characteristic)->second;
	}

	static std::optional<int> TryGetValue(int characterId, const game::Characteristic& characteristic)
	{
		return data::game::character::Characteristic::Read(characterId, (int)characteristic);
	}

	int Characteristic::GetValue() const
	{
		return TryGetValue(characterId, characteristic).value();
	}

	static const int OPPOSITION_BASE = 10;

	bool Characteristic::OpposedCheck(int opposition) const
	{
		const int CHECK_DIE_SIZE = 20;
		return common::RNG::Roll<CHECK_DIE_SIZE>() < (OPPOSITION_BASE + GetValue() - opposition);
	}

	void Characteristic::OnOpposedCheck(int opposition, std::function<void(bool)> callback) const
	{
		callback(OpposedCheck(opposition));
	}

	bool Characteristic::Check() const
	{
		return OpposedCheck(OPPOSITION_BASE);
	}

	void Characteristic::OnCheck(std::function<void(bool)> callback) const
	{
		OnOpposedCheck(10, callback);
	}
}