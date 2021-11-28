#pragma once
#include <optional>
namespace data::game::character
{
	struct Ship
	{
		int shipId;
		int berthType;
		//TODO: add characterid
		//TODO: merge with ShipCrew

		static void Write(int, const Ship&);
		static std::optional<Ship> ReadForCharacter(int);
	};
}