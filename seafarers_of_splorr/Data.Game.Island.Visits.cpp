#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Visits.h"
#include <string>
#include <format>
namespace data::game::island::Visits
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_VISITS = "Visits";
	const std::string FIELD_TURN = "Turn";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandVisits]([X] REAL NOT NULL, [Y] REAL NOT NULL, [Visits] INT NOT NULL, [Turn] INT NULL, UNIQUE([X],[Y]));";
	const std::string QUERY_ITEM = "SELECT [X],[Y],[Visits],[Turn] FROM [IslandVisits] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	const std::string REPLACE_ITEM = "REPLACE INTO [IslandVisits]([X],[Y],[Visits],[Turn]) VALUES({:.4f},{:.4f},{},{});";
	const std::string DELETE_ALL = "DELETE FROM [IslandVisits];";

	static void AutoCreateIslandVisitsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(const VisitData& data)
	{
		AutoCreateIslandVisitsTable();
		auto query = std::format(
			REPLACE_ITEM,
			data.location.GetX(),
			data.location.GetY(),
			data.visits,
			data::game::Common::OfOptionalInt(data.lastVisit));
		data::game::Common::Execute(query);
	}

	std::optional<VisitData> Read(const common::XY<double>& location)
	{
		AutoCreateIslandVisitsTable();
		auto query = std::format(
			QUERY_ITEM,
			location.GetX(),
			location.GetY());
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto& record = result.front();
			return std::optional<VisitData>({
				{common::Data::StringToDouble(record[FIELD_X]),common::Data::StringToDouble(record[FIELD_Y])},
				common::Data::StringToInt(record[FIELD_VISITS]),
				common::Data::StringToOptionalInt(record[FIELD_TURN])
				});
		}
		return std::nullopt;
	}

	void Clear()
	{
		AutoCreateIslandVisitsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}