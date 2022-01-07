#pragma once
#include "Game.Session.World.Profession.h"
#include <vector>
namespace game::session::world
{
	struct Professions
	{
		constexpr Professions() {}
		const std::vector<Profession>& GetProfessions() const;
		constexpr Profession GetProfession(const game::Profession& profession) { return Profession(profession); }
	};
}
