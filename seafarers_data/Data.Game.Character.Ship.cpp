#include "Data.Game.Character.Ship.h"
#include "Data.Game.Common.h"
#include <algorithm>
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterShips]
		(
			[CharacterId] INT NOT NULL UNIQUE, 
			[ShipId] INT NOT NULL,
			[Berth] INT NOT NULL,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([ShipId]) REFERENCES [Ships]([ShipId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [CharacterShips]
		(
			[CharacterId], 
			[ShipId], 
			[Berth]
		) 
		VALUES({},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM_FOR_CHARACTER =
		R"(SELECT 
			[ShipId],
			[Berth],
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view QUERY_ITEM_FOR_SHIP =
		R"(SELECT 
			[ShipId],
			[Berth],
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"sv;
	static constexpr std::string_view QUERY_CREW_FOR_SHIP =
		R"(SELECT 
			[CharacterId], 
			[Berth] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"sv;
	static constexpr std::string_view DELETE_ITEM =
		R"(DELETE FROM [CharacterShips] 
		WHERE 
			[CharacterId]={} 
			AND [ShipId]={};)"sv;
	static constexpr std::string_view QUERY_SHIP_FOR_CHARACTER =
		R"(SELECT 
			[ShipId] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view QUERY_BERTH_FOR_CHARACTER =
		R"(SELECT 
			[Berth] 
		FROM [CharacterShips] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view QUERY_CHARACTERS_FOR_SHIP =
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterShips] 
		WHERE 
			[ShipId]={};)"sv;

	static constexpr std::string_view FIELD_SHIP_ID = "ShipId";
	static constexpr std::string_view FIELD_BERTH = "Berth";
	static constexpr std::string_view FIELD_CHARACTER_ID = "CharacterId";

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
			FIELD_SHIP_ID);
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
			Common::DoToInt(FIELD_CHARACTER_ID));
		return result;
	}

	std::optional<int> Ship::ReadBerthForCharacter(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_BERTH_FOR_CHARACTER, characterId), 
			FIELD_BERTH);
	}
}