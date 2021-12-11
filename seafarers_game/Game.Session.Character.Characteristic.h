#pragma once
#include "Game.Characteristic.h"
#include <optional>
#include <string>
namespace game::session::character
{
	struct Characteristic
	{
		Characteristic(int, const game::Characteristic&);
		const std::string& GetName() const;
		std::optional<int> GetValue() const;
	private:
		int characterId;
		game::Characteristic characteristic;
	};
}