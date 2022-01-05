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

	std::vector<Mark> Marks::GetMarks() const
	{
		auto marks = MarkData::ReadMarkNames(characterId);
		std::vector<Mark> result;
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

	void Marks::AddMark(const std::string& name, const common::XY<double>& location) const
	{
		MarkData::WriteMark(characterId, name, location);
	}

	void Marks::RemoveMark(const std::string& name) const
	{
		MarkData::ClearMark(characterId, name);
	}

}