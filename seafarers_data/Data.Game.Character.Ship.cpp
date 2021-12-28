#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Ship.h"
#include "Data.Game.Ship.h"
#include "Data.Game.Common.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterShips]
		(
			[CharacterId] INT NOT NULL UNIQUE, 
			[ShipId] INT NOT NULL,
			[Berth] INT NOT NULL,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([ShipId]) REFERENCES [Ships]([ShipId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterShips]
		(
			[CharacterId], 
			[ShipId], 
			[Berth]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_ITEM_FOR_CHARACTER = 
		R"(SELECT 
			[ShipId],
			[Berth],
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string QUERY_ITEM_FOR_SHIP = 
		R"(SELECT 
			[ShipId],
			[Berth],
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string QUERY_CREW_FOR_SHIP = 
		R"(SELECT 
			[CharacterId], 
			[Berth] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string DELETE_ITEM =
		R"(DELETE FROM [CharacterShips] 
		WHERE 
			[CharacterId]={} 
			AND [ShipId]={};)"s;
	static const std::string QUERY_SHIP_FOR_CHARACTER =
		R"(SELECT 
			[ShipId] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string QUERY_BERTH_FOR_CHARACTER =
		R"(SELECT 
			[Berth] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string QUERY_CHARACTERS_FOR_SHIP =
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"s;

	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_BERTH = "Berth";
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

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
		auto record = Common::TryExecuteForOne(QUERY_SHIP_FOR_CHARACTER, characterId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_SHIP_ID);
		}
		return std::nullopt;
	}

	std::list<int> Ship::ReadCharactersForShip(int shipId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_CHARACTERS_FOR_SHIP, shipId);
		std::list<int> result;
		std::transform(
			records.begin(), 
			records.end(), 
			std::back_inserter(result), 
			[](const Common::Record& record) 
			{
				return Common::ToInt(record, FIELD_CHARACTER_ID);
			});
		return result;
	}

	std::optional<int> Ship::ReadBerthForCharacter(int characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_BERTH_FOR_CHARACTER, characterId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_BERTH);
		}
		return std::nullopt;
	}
}