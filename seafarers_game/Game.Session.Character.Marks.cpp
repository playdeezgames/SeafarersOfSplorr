#include <algorithm>
#include <Data.Game.Character.Mark.h>
#include "Game.Session.Character.Marks.h"
#include <iterator>
namespace game::session::character
{
	using MarkData = data::game::character::Mark;

	bool Marks::HasAny() const
	{
		return MarkData::HasAny(characterId);
	}

	std::list<Mark> Marks::GetMarks() const
	{
		auto marks = MarkData::ReadMarkNames(characterId);
		std::list<Mark> result;
		std::transform(
			marks.begin(),
			marks.end(), 
			std::back_inserter(result), 
			[this](const std::string& name) 
			{
				return Mark(characterId, name);
			});
		return result;
	}
}