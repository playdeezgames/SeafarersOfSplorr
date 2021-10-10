#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Fishery.h"
namespace data::game//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Fisheries]([FisheryId] INTEGER PRIMARY KEY AUTOINCREMENT,[FishType] INT NOT NULL,[X] REAL NOT NULL,[Y] REAL NOT NULL,[MoveX] REAL NOT NULL,[MoveY] REAL NOT NULL,[Radius] REAL NOT NULL,[Stock] INT NOT NULL,[Depletion] INT NOT NULL);";
	static const std::string INSERT_ITEM = "INSERT INTO [Fisheries]([FishType],[X],[Y],[MoveX],[MoveY],[Radius],[Stock],[Depletion]) VALUES({},{},{},{},{},{},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [Fisheries];";
	static const std::string QUERY_ALL = "SELECT [FisheryId],[FishType],[X],[Y],[MoveX],[MoveY],[Radius],[Stock],[Depletion] FROM [Fisheries];";
	static const std::string QUERY_BY_ID = "SELECT [FisheryId],[FishType],[X],[Y],[MoveX],[MoveY],[Radius],[Stock],[Depletion] FROM [Fisheries] WHERE [FisheryId]={};";
	static const std::string UPDATE_ITEM = "UPDATE [Fisheries] SET [X]={},[Y]={},[MoveX]={},[MoveY]={},[Stock]={},[Depletion]={} WHERE [FisheryId]={};";

	static const std::string FIELD_FISHERY_ID = "FisheryId";
	static const std::string FIELD_FISH_TYPE = "FishType";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_MOVE_X = "MoveX";
	static const std::string FIELD_MOVE_Y = "MoveY";
	static const std::string FIELD_RADIUS = "Radius";
	static const std::string FIELD_STOCK = "Stock";
	static const std::string FIELD_DEPLETION = "Depletion";

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
			INSERT_ITEM, 
			fishery.fishType,
			fishery.location.GetX(),
			fishery.location.GetY(),
			fishery.movement.GetX(),
			fishery.movement.GetY(),
			fishery.radius,
			fishery.stock,
			fishery.radius);
	}

	static Fishery ToFishery(const std::map<std::string, std::string>& record)
	{
		return {
			common::Data::ToInt(record.find(FIELD_FISHERY_ID)->second),
			common::Data::ToInt(record.find(FIELD_FISH_TYPE)->second),
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			{
				common::Data::ToDouble(record.find(FIELD_MOVE_X)->second),
				common::Data::ToDouble(record.find(FIELD_MOVE_Y)->second)
			},
			common::Data::ToDouble(record.find(FIELD_RADIUS)->second),
			common::Data::ToInt(record.find(FIELD_STOCK)->second),
			common::Data::ToInt(record.find(FIELD_DEPLETION)->second)
		};
	}

	std::list<Fishery> Fishery::All()
	{
		AutoCreateFisheryTable();
		std::list<Fishery> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back(ToFishery(record));
		}
		return result;
	}

	void Fishery::Write(const Fishery& fishery)
	{
		AutoCreateFisheryTable();
		Common::Execute(
			UPDATE_ITEM,
			fishery.location.GetX(), 
			fishery.location.GetY(), 
			fishery.movement.GetX(), 
			fishery.movement.GetY(), 
			fishery.stock,
			fishery.depletion,
			fishery.fisheryId);
	}

	std::optional<Fishery> Fishery::Read(int fisheryId)
	{
		AutoCreateFisheryTable();
		auto records = Common::Execute(QUERY_BY_ID,fisheryId);
		if(!records.empty())
		{
			return (ToFishery(records.front()));
		}
		return std::nullopt;
	}
}