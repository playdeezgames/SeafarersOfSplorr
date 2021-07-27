#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Known.h"
#include <format>
#include <string>
namespace data::game::island::Known
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [KnownIslands]([X] REAL NOT NULL, [Y] REAL NOT NULL, UNIQUE([X],[Y]));";
	const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [KnownIslands] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	const std::string QUERY_ALL = "SELECT [X],[Y] FROM [KnownIslands];";
	const std::string REPLACE_ITEM = "REPLACE INTO [KnownIslands]([X],[Y]) VALUES({:.4f},{:.4f});";
	const std::string DELETE_ALL = "DELETE FROM [KnownIslands];";

	static void AutoCreateKnownIslandsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(const common::XY<double>& location)
	{
		AutoCreateKnownIslandsTable();
		auto query = std::format(REPLACE_ITEM, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}

	bool Read(const common::XY<double>& location)
	{
		AutoCreateKnownIslandsTable();
		auto query = std::format(QUERY_ITEM, location.GetX(), location.GetY());
		auto result = data::game::Common::Execute(query);
		return !result.empty();
	}

	void Clear()
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::list<common::XY<double>> All()
	{
		AutoCreateKnownIslandsTable();
		auto query = std::format(QUERY_ALL);
		auto records = data::game::Common::Execute(query);
		std::list<common::XY<double>> result;
		for (auto& record : records)
		{
			result.push_back({
				common::Data::StringToDouble(record[FIELD_X]),
				common::Data::StringToDouble(record[FIELD_Y])
				});
		}
		return result;
	}

}