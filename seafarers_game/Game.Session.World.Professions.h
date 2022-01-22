#pragma once
#include "Game.Session.World.Profession.h"
#include <vector>
namespace game::session::world
{
	struct Professions
	{
		Professions() = delete;
		static const std::vector<Profession>& GetProfessions();
		static constexpr Profession GetProfession(const game::Profession& profession) { 
			return Profession(profession); }
	};
}
