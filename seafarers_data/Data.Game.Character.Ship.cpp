#include <Common.Data.h>
#include "Data.Game.Character.Ship.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include "Data.Game.Ship.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterShips]([CharacterId] INT NOT NULL UNIQUE, [ShipId] INT NOT NULL,[BerthType] INT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterShips]([CharacterId], [ShipId], [BerthType]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [ShipId],[BerthType] FROM [CharacterShips] WHERE [CharacterId]={};";
	static const std::string QUERY_CREW_FOR_SHIP = "SELECT [CharacterId], [BerthType] FROM [CharacterShips] WHERE [ShipId]={};";

	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_BERTH_TYPE = "BerthType";
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

	static auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void Ship::Write(int characterId, const Ship& ship)
	{
		AutoCreateTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			ship.shipId, 
			ship.berthType);
	}

	static Ship ToShip(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_SHIP_ID)->second),
			common::Data::ToInt(record.find(FIELD_BERTH_TYPE)->second)
		};
	}

	std::optional<Ship> Ship::Read(int characterId)
	{
		AutoCreateTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return ToShip(records.front());
		}
		return std::nullopt;
	}
}