#pragma once
#include "Game.Characteristic.h"
#include <string>
namespace game
{
	struct Characteristics
	{
		static const std::string& GetName(const Characteristic&);
	};
}
