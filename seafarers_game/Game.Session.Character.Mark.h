#pragma once
#include <Common.XY.h>
#include <string>
namespace game::session::character
{
	struct Mark
	{
		constexpr explicit Mark(int characterId, const std::string_view& name) : characterId(characterId), name(name) {}
		common::XY<double> GetLocation() const;
		const std::string_view& GetName() const { return name; }
	private:
		int characterId;
		std::string_view name;
	};
}