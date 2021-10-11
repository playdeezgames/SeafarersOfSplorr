#include <Common.Data.h>
#include "Data.Game.Avatar.ShipCrew.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include "Data.Game.Ship.h"
namespace data::game::avatar//20211011
{
	static const std::string QUERY_CREW_FOR_SHIP = "SELECT [AvatarId], [BerthType] FROM [AvatarShips] WHERE [ShipId]={};";

	static const std::string FIELD_BERTH_TYPE = "BerthType";
	static const std::string FIELD_AVATAR_ID = "AvatarId";

	void AutoCreateAvatarShipTable();

	static ShipCrew ToShipCrew(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_AVATAR_ID)->second),
			common::Data::ToInt(record.find(FIELD_BERTH_TYPE)->second)
		};
	}

	std::vector<ShipCrew> ShipCrew::Read(int shipId)
	{
		AutoCreateAvatarShipTable();
		std::vector<ShipCrew> result;
		auto records = Common::Execute(QUERY_CREW_FOR_SHIP, shipId);
		for (auto& record : records)
		{
			result.push_back(ToShipCrew(record));
		}
		return result;
	}
}