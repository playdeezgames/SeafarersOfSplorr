#include "Game.Session.World.Professions.h"
namespace game::session::world
{
	static const std::vector<Profession> professions =
	{
		Profession(game::Profession::HUNTER),
		Profession(game::Profession::SAILOR),
		Profession(game::Profession::THIEF),
		Profession(game::Profession::WARRIOR)
	};

	const std::vector<Profession>& Professions::GetProfessions() const
	{
		return professions;
	}
}