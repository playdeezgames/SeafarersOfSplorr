#pragma once
#include <Common.XY.h>
#include <string>
namespace game::session::character
{
	struct Mark
	{
		constexpr Mark(int characterId, const std::string& name) : characterId(characterId), name(name) {}
		common::XY<double> GetLocation() const;
		const std::string& GetName() const { return name; }
	private:
		int characterId;
		std::string name;
	};
}