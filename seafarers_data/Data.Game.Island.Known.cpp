#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Known.h"
#include <string>
namespace data::game::island
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [KnownIslands]([X] REAL NOT NULL, [Y] REAL NOT NULL, UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [KnownIslands] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	static const std::string QUERY_ALL = "SELECT [X],[Y] FROM [KnownIslands];";
	static const std::string REPLACE_ITEM = "REPLACE INTO [KnownIslands]([X],[Y]) VALUES({:.4f},{:.4f});";
	static const std::string DELETE_ALL = "DELETE FROM [KnownIslands];";

	static const auto AutoCreateKnownIslandsTable = data::game::Common::Run(CREATE_TABLE);

	void Known::Write(const common::XY<double>& location)
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(REPLACE_ITEM, location.GetX(), location.GetY());
	}

	bool Known::Read(const common::XY<double>& location)
	{
		AutoCreateKnownIslandsTable();
		auto result = data::game::Common::Execute(QUERY_ITEM, location.GetX(), location.GetY());
		return !result.empty();
	}

	void Known::Clear()
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::list<common::XY<double>> Known::All()
	{
		AutoCreateKnownIslandsTable();
		auto records = data::game::Common::Execute(QUERY_ALL);
		std::list<common::XY<double>> result;
		for (auto& record : records)
		{
			result.push_back({
				common::Data::ToDouble(record[FIELD_X]),
				common::Data::ToDouble(record[FIELD_Y])
				});
		}
		return result;
	}
}