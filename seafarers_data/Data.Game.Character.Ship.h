#pragma once
#include <list>
#include <optional>
#include <vector>
namespace data::game::character
{
	struct Ship
	{
		Ship() = delete;
		static void Initialize();
		static void Write(int, int, int);
		static void Clear(int, int);
		static std::optional<int> ReadShipForCharacter(int);
		static std::list<int> ReadCharactersForShip(int);
		static std::optional<int> ReadBerthForCharacter(int);
	};
}