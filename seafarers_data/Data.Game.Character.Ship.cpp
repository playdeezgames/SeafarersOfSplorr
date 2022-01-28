#include "Data.Game.Character.Ship.h"
#include "Data.Game.Common.h"
#include <algorithm>
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterShips]
		(
			[CharacterId] INT NOT NULL UNIQUE, 
			[ShipId] INT NOT NULL,
			[Berth] INT NOT NULL,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([ShipId]) REFERENCES [Ships]([ShipId])
		);)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [CharacterShips]
		(
			[CharacterId], 
			[ShipId], 
			[Berth]
		) 
		VALUES({},{},{});)"sv;
	static constexpr auto QUERY_ITEM_FOR_CHARACTER =
		R"(SELECT 
			[ShipId],
			[Berth],
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto QUERY_ITEM_FOR_SHIP =
		R"(SELECT 
			[ShipId],
			[Berth],
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"sv;
	static constexpr auto QUERY_CREW_FOR_SHIP =
		R"(SELECT 
			[CharacterId], 
			[Berth] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"sv;
	static constexpr auto DELETE_ITEM =
		R"(DELETE FROM [CharacterShips] 
		WHERE 
			[CharacterId]={} 
			AND [ShipId]={};)"sv;
	static constexpr auto QUERY_SHIP_FOR_CHARACTER =
		R"(SELECT 
			[ShipId] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto QUERY_BERTH_FOR_CHARACTER =
		R"(SELECT 
			[Berth] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto QUERY_CHARACTERS_FOR_SHIP =
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"sv;
	static constexpr auto QUERY_CHARACTERS_FOR_SHIP_AND_BERTH =
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={}
			AND [Berth]={};)"sv;
	static constexpr auto COLUMN_SHIP_ID = "ShipId"sv;
	static constexpr auto COLUMN_BERTH = "Berth"sv;
	static constexpr auto COLUMN_CHARACTER_ID = "CharacterId"sv;

	void Ship::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	void Ship::Write(int characterId, int shipId, int berth)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, shipId, berth);
	}

	void Ship::Clear(int characterId, int shipId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, shipId);
	}

	std::optional<int> Ship::ReadShipForCharacter(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_SHIP_FOR_CHARACTER, characterId), 
			COLUMN_SHIP_ID);
	}

	std::vector<int> Ship::ReadCharactersForShip(int shipId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_CHARACTERS_FOR_SHIP, shipId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(COLUMN_CHARACTER_ID));
		return result;
	}

	std::optional<int> Ship::ReadBerthForCharacter(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_BERTH_FOR_CHARACTER, characterId), 
			COLUMN_BERTH);
	}

	std::vector<int> Ship::ReadCharactersForShipByBerthType(int shipId, int berth)
	{
		Initialize();
		auto records = Common::Execute(QUERY_CHARACTERS_FOR_SHIP_AND_BERTH, shipId, berth);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(COLUMN_CHARACTER_ID));
		return result;
	}
}