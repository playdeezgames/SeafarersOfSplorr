#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Visit.h"
#include <string>
namespace data::game::island//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandVisits]([X] REAL NOT NULL, [Y] REAL NOT NULL, [Visits] INT NOT NULL, [Turn] INT NULL, UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[Visits],[Turn] FROM [IslandVisits] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandVisits]([X],[Y],[Visits],[Turn]) VALUES({:.4f},{:.4f},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [IslandVisits];";

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_VISITS = "Visits";
	static const std::string FIELD_TURN = "Turn";

	static const auto AutoCreateIslandVisitsTable = data::game::Common::Run(CREATE_TABLE);

	void Visit::Write(const Visit& data)
	{
		AutoCreateIslandVisitsTable();
		data::game::Common::Execute(
			REPLACE_ITEM,
			data.location.GetX(),
			data.location.GetY(),
			data.visits,
			common::Data::OfOptional(data.lastVisit));
	}

	static Visit ToVisit(const std::map<std::string, std::string>& record)
	{
		return
		{
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			common::Data::ToInt(record.find(FIELD_VISITS)->second),
			common::Data::ToOptionalInt(record.find(FIELD_TURN)->second)
		};
	}

	std::optional<Visit> Visit::Read(const common::XY<double>& location)
	{
		AutoCreateIslandVisitsTable();
		auto records = data::game::Common::Execute(
			QUERY_ITEM,
			location.GetX(),
			location.GetY());
		if (!records.empty())
		{
			return ToVisit(records.front());
		}
		return std::nullopt;
	}

	void Visit::Clear()
	{
		AutoCreateIslandVisitsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}