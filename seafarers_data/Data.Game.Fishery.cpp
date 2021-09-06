#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Fishery.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Fisheries]([FisheryId] INTEGER PRIMARY KEY AUTOINCREMENT,[FishType] INT NOT NULL,[X] REAL NOT NULL,[Y] REAL NOT NULL,[MoveX] REAL NOT NULL,[MoveY] REAL NOT NULL,[Radius] REAL NOT NULL);";
	static const std::string INSERT_ITEM = "INSERT INTO [Fisheries]([FishType],[X],[Y],[MoveX],[MoveY],[Radius]) VALUES({},{},{},{},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [Fisheries];";

	static const auto AutoCreateFisheryTable = Common::Run(CREATE_TABLE);

	void Fishery::Clear()
	{
		AutoCreateFisheryTable();
		Common::Execute(DELETE_ALL);
	}

	void Fishery::Add(const Fishery& fishery)
	{
		AutoCreateFisheryTable();
		Common::Execute(
			std::format(
				INSERT_ITEM, 
				fishery.fishType,
				fishery.location.GetX(),
				fishery.location.GetY(),
				fishery.movement.GetX(),
				fishery.movement.GetY(),
				fishery.radius));
	}
}