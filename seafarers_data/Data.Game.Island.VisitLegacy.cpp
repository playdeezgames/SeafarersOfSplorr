#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.VisitLegacy.h"
namespace data::game::island
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [IslandVisitsLegacy]"
		"("
			"[IslandId] INT NOT NULL UNIQUE, "
			"[Visits] INT NOT NULL, "
			"[Turn] INT NULL"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[IslandId],"
			"[Visits],"
			"[Turn] "
		"FROM [IslandVisitsLegacy] "
		"WHERE "
			"[IslandId]={};";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [IslandVisitsLegacy]"
		"("
			"[IslandId],"
			"[Visits],"
			"[Turn]"
		") "
		"VALUES({},{},{});";
	static const std::string DELETE_ALL = 
		"DELETE FROM [IslandVisitsLegacy];";

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_VISITS = "Visits";
	static const std::string FIELD_TURN = "Turn";

	static const auto AutoCreateIslandVisitsTable = data::game::Common::Run(CREATE_TABLE);

	void VisitLegacy::Write(const VisitLegacy& data)
	{
		AutoCreateIslandVisitsTable();
		data::game::Common::Execute(
			REPLACE_ITEM,
			data.islandId,
			data.visits,
			common::Data::OfOptional(data.lastVisit));
	}

	static VisitLegacy ToVisit(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_ISLAND_ID)->second),
			common::Data::ToInt(record.find(FIELD_VISITS)->second),
			common::Data::ToOptionalInt(record.find(FIELD_TURN)->second)
		};
	}

	std::optional<VisitLegacy> VisitLegacy::Read(int islandId)
	{
		AutoCreateIslandVisitsTable();
		auto records = data::game::Common::Execute(
			QUERY_ITEM,
			islandId);
		if (!records.empty())
		{
			return ToVisit(records.front());
		}
		return std::nullopt;
	}

	void VisitLegacy::Clear()
	{
		AutoCreateIslandVisitsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}