#include <Common.Data.h>
#include "Data.Game.Character.ShipCrew.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include "Data.Game.Ship.h"
namespace data::game::character
{
	static const std::string QUERY_CREW_FOR_SHIP = "SELECT [CharacterId], [BerthType] FROM [AvatarShips] WHERE [ShipId]={};";

	static const std::string FIELD_BERTH_TYPE = "BerthType";
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

	//TODO: these two lines are duplicated!
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterShips]([CharacterId] INT NOT NULL UNIQUE, [ShipId] INT NOT NULL,[BerthType] INT NOT NULL);";
	static auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);


	static ShipCrew ToShipCrew(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_CHARACTER_ID)->second),
			common::Data::ToInt(record.find(FIELD_BERTH_TYPE)->second)
		};
	}

	std::vector<ShipCrew> ShipCrew::Read(int shipId)
	{
		AutoCreateTable();
		std::vector<ShipCrew> result;
		auto records = Common::Execute(QUERY_CREW_FOR_SHIP, shipId);
		for (auto& record : records)
		{
			result.push_back(ToShipCrew(record));
		}
		return result;
	}
}