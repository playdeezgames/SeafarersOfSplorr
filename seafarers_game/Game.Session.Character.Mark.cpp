#include <Data.Game.Character.Mark.h>
#include "Game.Session.Character.Mark.h"
namespace game::session::character
{
	using MarkData = data::game::character::Mark;

	common::XY<double> Mark::GetLocation() const
	{
		return MarkData::ReadMark(characterId, name).value();
	}
}