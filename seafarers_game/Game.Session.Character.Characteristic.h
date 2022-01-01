#pragma once
#include <functional>
#include "Game.Characteristic.h"
#include <optional>
#include <string>
namespace game::session::character
{
	struct Characteristic
	{
		constexpr explicit Characteristic(
			int characterId, 
			const game::Characteristic& characteristic)
			: characterId(characterId)
			, characteristic(characteristic) { }
		const std::string& GetName() const;
		int GetValue() const;
		bool Check() const;
		void OnCheck(std::function<void(bool)> callback) const;
		bool OpposedCheck(int opposition) const;
		void OnOpposedCheck(int opposition, std::function<void(bool)> callback) const;
	private:
		int characterId;
		game::Characteristic characteristic;
	};
}