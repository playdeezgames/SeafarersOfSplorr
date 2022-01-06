#pragma once
#include "Game.Profession.h"
namespace game::session::world
{
	struct Profession
	{
		constexpr Profession(const game::Profession profession) : profession(profession) {}
	private:
		game::Profession profession;
	};
}
