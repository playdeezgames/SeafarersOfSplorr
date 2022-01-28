#pragma once
#include <optional>
#include <vector>
namespace data::game::character
{
	struct Ship
	{
		Ship() = delete;
		static void Initialize();
		static void Write(int characterId, int shipId, int berth);
		static void Clear(int characterId, int shipId);
		static std::optional<int> ReadShipForCharacter(int characterId);
		static std::optional<int> ReadBerthForCharacter(int characterId);
		static std::vector<int> ReadCharactersForShip(int shipId);
		static std::vector<int> ReadCharactersForShipByBerthType(int shipId, int berth);
	};
}