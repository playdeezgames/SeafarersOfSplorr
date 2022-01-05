#pragma once
#include "Game.Session.Character.Mark.h"
#include <vector>
namespace game::session::character
{
	struct Marks
	{
		constexpr Marks(int characterId) : characterId(characterId) {}
		constexpr Mark GetMark(const std::string& name) const { return Mark(characterId, name); }
		std::vector<Mark> GetMarks() const;
		bool HasAny() const;
		void AddMark(const std::string& name, const common::XY<double>& location) const;
		void RemoveMark(const std::string& name) const;
	private:
		int characterId;
	};
}
