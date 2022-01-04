#pragma once
#include "Game.Session.Character.Mark.h"
#include <list>
namespace game::session::character
{
	struct Marks
	{
		constexpr Marks(int characterId) : characterId(characterId) {}
		std::list<Mark> GetMarks() const;
		bool HasAny() const;
		void AddMark(const std::string& name, const common::XY<double>& location) const;
	private:
		int characterId;
	};
}
