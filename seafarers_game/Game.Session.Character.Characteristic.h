#pragma once
#include <functional>
#include "Game.Characteristic.h"
#include <optional>
#include <string>
namespace game::session::character
{
	struct Characteristic
	{
		Characteristic(int, const game::Characteristic&);
		const std::string& GetName() const;
		int GetValue() const;
		bool Check() const;
		void OnCheck(std::function<void(bool)>) const;
		bool OpposedCheck(int) const;
		void OnOpposedCheck(int, std::function<void(bool)>) const;
	private:
		int characterId;
		game::Characteristic characteristic;
	};
}