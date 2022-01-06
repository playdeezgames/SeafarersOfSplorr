#pragma once
#include "Game.Profession.h"
#include <string>
namespace game::session::world
{
	struct Profession
	{
		constexpr Profession(const game::Profession profession) : profession(profession) {}
		std::string GetName() const;
	private:
		game::Profession profession;
	};
}
