#include <Common.Data.h>
#include <Common.Utility.List.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Islands]
		(
			[IslandId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[X] REAL NOT NULL,
			[Y] REAL NOT NULL,
			[Name] TEXT NOT NULL,
			[PatronDemigodId] INT NOT NULL,
			UNIQUE([X],[Y])
		);)"s;
	static const std::string QUERY_ITEM_XY =
		R"(SELECT 
			[X],
			[Y]
		FROM [Islands] 
		WHERE 
		[IslandId]={};)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [Islands] 
		WHERE 
		[IslandId]={};)"s;
	static const std::string QUERY_ID_FOR_LOCATION =
		"SELECT "
			"[IslandId] "
		"FROM [Islands] "
		"WHERE "
			"[X]={:.4f} "
			"AND [Y]={:.4f};";
	static const std::string INSERT_ITEM = 
		"INSERT INTO [Islands]"
		"("
			"[X],"
			"[Y],"
			"[Name],"
			"[PatronDemigodId]"
		") "
		"VALUES ({:.4f},{:.4f},{},{});";
	static const std::string UPDATE_ITEM = 
		"UPDATE [Islands] "
		"SET "
		"[X]={:.4f},"
		"[Y]={:.4f},"
		"[Name]={},"
		"[PatronDemigodId]={}"
		"WHERE "
			"[IslandId]={};";
	static const std::string QUERY_ALL = 
		"SELECT "
			"[IslandId],"
			"[X],"
			"[Y],"
			"[Name],"
			"[PatronDemigodId] "
		"FROM [Islands];";
	static const std::string CLEAR_ALL = 
		"DELETE FROM [Islands];";

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_DEMIGOD_ID = "PatronDemigodId";

	void Island::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	int Island::Create(const common::XY<double>& location, const std::string& name, int patronDemigodId)
	{
		data::game::Common::Execute(
			INSERT_ITEM,
			location.GetX(),
			location.GetY(),
			common::Data::QuoteString(name),
			patronDemigodId);
		return Common::LastInsertedIndex();
	}

	std::list<int> Island::All()
	{
		Initialize();
		return common::utility::List::Map<Common::Record, int>(
			Common::DoExecute(QUERY_ALL), 
			[](Common::Record record) 
			{
				return Common::ToInt(record, FIELD_ISLAND_ID); 
			});
	}

	void Island::Clear()
	{
		Initialize();
		data::game::Common::Execute(CLEAR_ALL);
	}

	std::optional<common::XY<double>> Island::ReadLocation(int islandId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_XY, islandId);
		if (record)
		{
			return Common::ToXY(record.value());
		}
		return std::nullopt;
	}

	std::optional<std::string> Island::ReadName(int islandId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, islandId);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	std::optional<int> Island::ReadPatronDemigodId(int islandId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_PATRON_DEMIGOD_ID, islandId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_PATRON_DEMIGOD_ID);
		}
		return std::nullopt;
	}
}