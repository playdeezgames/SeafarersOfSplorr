#pragma once
#include "Game.Profession.h"
#include <string>
namespace game::session::world
{
	struct Profession
	{
		constexpr explicit Profession(const game::Profession profession) : profession(profession) {}
		std::string GetName() const;
		constexpr explicit operator game::Profession() const { return profession; }
	private:
		game::Profession profession;
	};
}
