#include <Data.Game.Demigod.h>
#include "Game.Session.Demigod.h"
namespace game::session
{
	std::string Demigod::GetName() const
	{
		return data::game::Demigod::ReadName(demigodId).value();
	}
}