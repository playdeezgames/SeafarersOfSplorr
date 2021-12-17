#include <Common.RNG.h>
#include <Data.Game.Character.Characteristic.h>
#include "Game.Session.Character.Characteristic.h"
namespace game::session::character
{
	Characteristic::Characteristic(int characterId, const game::Characteristic& characteristic)
		: characterId(characterId)
		, characteristic(characteristic)
	{

	}

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

	bool Characteristic::OpposedCheck(int opposition) const
	{
		return common::RNG::FromRange(1, 21) < (10 + GetValue() - opposition);//d20 < (10 + my characteristic - opposed characteristic)
	}

	void Characteristic::OnOpposedCheck(int opposition, std::function<void(bool)> callback) const
	{
		callback(OpposedCheck(opposition));
	}

	bool Characteristic::Check() const
	{
		return OpposedCheck(10);
	}

	void Characteristic::OnCheck(std::function<void(bool)> callback) const
	{
		OnOpposedCheck(10, callback);
	}
}