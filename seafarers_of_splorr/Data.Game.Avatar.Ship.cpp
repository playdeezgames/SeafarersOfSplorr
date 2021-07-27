#include "Common.Data.h"
#include "Data.Game.Avatar.Ship.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Ship
{
	const std::string FIELD_SHIP_ID = "ShipId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarShips]([AvatarId] INT NOT NULL UNIQUE, [ShipId] INT NOT NULL);";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarShips]([AvatarId], [ShipId]) VALUES({},{});";
	const std::string QUERY_ITEM = "SELECT [ShipId] FROM [AvatarShips] WHERE [AvatarId]={};";

	static void AutoCreateAvatarShipTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(int ship)
	{
		AutoCreateAvatarShipTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, ship));
	}

	int Read()
	{
		AutoCreateAvatarShipTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, data::game::Common::AVATAR_ID));
		return common::Data::StringToInt(records.front()[FIELD_SHIP_ID]);
	}
}