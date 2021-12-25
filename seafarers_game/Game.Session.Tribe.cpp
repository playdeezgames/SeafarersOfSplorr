#include <Data.Game.Tribe.h>
#include "Game.Session.Tribe.h"
namespace game::session
{
	using TribeData = data::game::Tribe;
	std::string Tribe::GetName() const
	{
		return TribeData::ReadName(tribeId).value();
	}
}