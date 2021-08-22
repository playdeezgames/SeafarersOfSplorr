#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include <format>
namespace data::game::Island
{
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Islands]([X] REAL NOT NULL,[Y] REAL NOT NULL,[CareeningDistance] REAL NOT NULL,[Name] TEXT NOT NULL,UNIQUE([X],[Y]));";
	const std::string QUERY_ITEM = "SELECT [X],[Y],[Name],[CareeningDistance] FROM [Islands] WHERE [X] = {:.4f} AND [Y]={:.4f};";
	const std::string REPLACE_ITEM = "REPLACE INTO [Islands]([X],[Y],[Name],[CareeningDistance]) VALUES ({:.4f},{:.4f},{},{});";
	const std::string QUERY_ALL = "SELECT [X],[Y],[Name],[CareeningDistance] FROM [Islands];";
	const std::string CLEAR_ALL = "DELETE FROM [Islands];";
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_NAME = "Name";
	const std::string FIELD_CAREENING_DISTANCE = "CareeningDistance";

	const auto AutoCreateIslandTable = data::game::Common::Run(CREATE_TABLE);

	void Write(const Data& data)
	{
		AutoCreateIslandTable();
		auto query =
			std::format(
				REPLACE_ITEM,
				data.location.GetX(), 
				data.location.GetY(),
				common::Data::QuoteString(data.name),
				data.careeningDistance);
		data::game::Common::Execute(query);
	}

	static Data ToIslandData(const std::map<std::string, std::string> record)
	{
		Data data =
		{
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			record.find(FIELD_NAME)->second,
			common::Data::ToDouble(record.find(FIELD_CAREENING_DISTANCE)->second)
		};
		return data;
	}

	std::optional<Data> Read(const common::XY<double>& location)
	{
		AutoCreateIslandTable();
		auto query =
			std::format(
				QUERY_ITEM,
				location.GetX(), location.GetY());
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			return ToIslandData(result.front());
		}
		return std::nullopt;
	}

	std::list<Data> All()
	{
		AutoCreateIslandTable();
		auto records = data::game::Common::Execute(QUERY_ALL);
		std::list<Data> result;
		for (auto& record : records)
		{
			result.push_back(ToIslandData(record));
		}
		return result;
	}

	void Clear()
	{
		AutoCreateIslandTable();
		data::game::Common::Execute(CLEAR_ALL);
	}
}