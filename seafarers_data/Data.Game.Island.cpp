#include <Common.Data.h>
#include <Common.Utility.h>
#include <Common.Utility.List.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
namespace data::game//20211011
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Islands]([X] REAL NOT NULL,[Y] REAL NOT NULL,[Name] TEXT NOT NULL,[MerchantSpawnRate] REAL NOT NULL,[PatronDemigodId] INT NOT NULL,UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[Name],[MerchantSpawnRate],[PatronDemigodId] FROM [Islands] WHERE [X] = {:.4f} AND [Y]={:.4f};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Islands]([X],[Y],[Name],[MerchantSpawnRate],[PatronDemigodId]) VALUES ({:.4f},{:.4f},{},{},{});";
	static const std::string QUERY_ALL = "SELECT [X],[Y],[Name],[MerchantSpawnRate],[PatronDemigodId] FROM [Islands];";
	static const std::string CLEAR_ALL = "DELETE FROM [Islands];";

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_MERCHANT_SPAWN_RATE = "MerchantSpawnRate";
	static const std::string FIELD_PATRON_DEMIGOD_ID = "PatronDemigodId";

	static const auto AutoCreateIslandTable = data::game::Common::Run(CREATE_TABLE);

	void Island::Write(const Island& data)
	{
		AutoCreateIslandTable();
		data::game::Common::Execute(
			REPLACE_ITEM,
			data.location.GetX(),
			data.location.GetY(),
			common::Data::QuoteString(data.name),
			data.merchantSpawnRate,
			data.patronDemigodId);
	}

	static Island ToIsland(const std::map<std::string, std::string> record)
	{
		Island data =
		{
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			record.find(FIELD_NAME)->second,
			common::Data::ToDouble(record.find(FIELD_MERCHANT_SPAWN_RATE)->second),
			common::Data::ToInt(record.find(FIELD_PATRON_DEMIGOD_ID)->second)
		};
		return data;
	}

	std::optional<Island> Island::Read(const common::XY<double>& location)
	{
		AutoCreateIslandTable();
		auto result = data::game::Common::Execute(
			QUERY_ITEM,
			location.GetX(),
			location.GetY());
		if (!result.empty())
		{
			return ToIsland(result.front());
		}
		return std::nullopt;
	}

	std::list<Island> Island::Filter(std::function<bool(const Island&)> filter)
	{
		AutoCreateIslandTable();
		return common::utility::List::Map<Common::Record, Island>(Common::DoExecute(QUERY_ALL), ToIsland, filter);
	}


	std::list<Island> Island::All()
	{
		AutoCreateIslandTable();
		return common::utility::List::Map<Common::Record, Island>(Common::DoExecute(QUERY_ALL), ToIsland);
	}

	void Island::Clear()
	{
		AutoCreateIslandTable();
		data::game::Common::Execute(CLEAR_ALL);
	}
}