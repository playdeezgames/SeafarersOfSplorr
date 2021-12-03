#include <Common.Data.h>
#include <Common.Utility.List.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Islands]([IslandId] INTEGER PRIMARY KEY AUTOINCREMENT, [X] REAL NOT NULL,[Y] REAL NOT NULL,[Name] TEXT NOT NULL,[PatronDemigodId] INT NOT NULL,UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [IslandId],[X],[Y],[Name],[PatronDemigodId] FROM [Islands] WHERE [IslandId]={};";
	static const std::string QUERY_ID_FOR_LOCATION = "SELECT [IslandId] FROM [Islands] WHERE [X]={:.4f} AND [Y]={:.4f};";
	static const std::string INSERT_ITEM = "INSERT INTO [Islands]([X],[Y],[Name],[PatronDemigodId]) VALUES ({:.4f},{:.4f},{},{});";
	static const std::string UPDATE_ITEM = "UPDATE [Islands] SET [X]={:.4f},[Y]={:.4f},[Name]={},[PatronDemigodId]={}) WHERE [IslandId]={};";
	static const std::string QUERY_ALL = "SELECT [IslandId],[X],[Y],[Name],[PatronDemigodId] FROM [Islands];";
	static const std::string CLEAR_ALL = "DELETE FROM [Islands];";

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_DEMIGOD_ID = "PatronDemigodId";

	static const auto AutoCreateIslandTable = data::game::Common::Run(CREATE_TABLE);

	int Island::Write(const Island& data)
	{
		AutoCreateIslandTable();
		if (data.id == 0)
		{
			data::game::Common::Execute(
				INSERT_ITEM,
				data.location.GetX(),
				data.location.GetY(),
				common::Data::QuoteString(data.name),
				data.patronDemigodId);
			return Common::LastInsertedIndex();
		}
		else
		{
			data::game::Common::Execute(
				UPDATE_ITEM,
				data.location.GetX(),
				data.location.GetY(),
				common::Data::QuoteString(data.name),
				data.patronDemigodId,
				data.id);
			return data.id;
		}
	}

	static Island ToIsland(const std::map<std::string, std::string>& record)
	{
		Island data =
		{
			common::Data::ToInt(record.find(FIELD_ISLAND_ID)->second),
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			record.find(FIELD_NAME)->second,
			common::Data::ToInt(record.find(FIELD_PATRON_DEMIGOD_ID)->second)
		};
		return data;
	}

	std::optional<Island> Island::Read(int islandId)
	{
		AutoCreateIslandTable();
		auto result = data::game::Common::Execute(
			QUERY_ITEM,
			islandId);
		if (!result.empty())
		{
			return ToIsland(result.front());
		}
		return std::nullopt;
	}

	std::optional<int> Island::Find(const common::XY<double>& location)
	{
		AutoCreateIslandTable();
		auto result = data::game::Common::Execute(
			QUERY_ID_FOR_LOCATION,
			location.GetX(),
			location.GetY());
		if (!result.empty())
		{
			return common::Data::ToInt(result.front()[FIELD_ISLAND_ID]);
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