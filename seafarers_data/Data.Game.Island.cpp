#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
#include "Data.Game.Island.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Islands]
		(
			[IslandId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[X] REAL NOT NULL,
			[Y] REAL NOT NULL,
			[Name] TEXT NOT NULL,
			[PatronDemigodId] INT NOT NULL,
			UNIQUE([X],[Y]),
			FOREIGN KEY ([PatronDemigodId]) REFERENCES [Demigods]([DemigodId])
		);)"sv;
	static constexpr std::string_view QUERY_ITEM_XY =
		R"(SELECT 
			[X],
			[Y]
		FROM [Islands] 
		WHERE 
		[IslandId]={};)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [Islands] 
		WHERE 
		[IslandId]={};)"sv;
	static constexpr std::string_view QUERY_ID_FOR_LOCATION =
		R"(SELECT 
			[IslandId] 
		FROM [Islands] 
		WHERE 
			[X]={:.4f} 
			AND [Y]={:.4f};)"sv;
	static constexpr std::string_view INSERT_ITEM =
		R"(INSERT INTO [Islands]
		(
			[X],
			[Y],
			[Name],
			[PatronDemigodId]
		) 
		VALUES ({:.4f},{:.4f},{},{});)"sv;
	static constexpr std::string_view QUERY_ALL =
		R"(SELECT 
			[IslandId]
		FROM [Islands];)"sv;
	static constexpr std::string_view QUERY_ALL_LOCATIONS =
		R"(SELECT 
			[IslandId],
			[X],
			[Y]
		FROM [Islands];)"sv;
	static constexpr std::string_view CLEAR_ALL =
		R"(DELETE FROM [Islands];)"sv;

	static constexpr std::string_view FIELD_ISLAND_ID = "IslandId";
	static constexpr std::string_view FIELD_X = "X";
	static constexpr std::string_view FIELD_Y = "Y";
	static constexpr std::string_view FIELD_NAME = "Name";
	static constexpr std::string_view FIELD_PATRON_DEMIGOD_ID = "PatronDemigodId";

	void Island::Initialize()
	{
		Demigod::Initialize();
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

	std::vector<int> Island::All()
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ISLAND_ID));
		return result;
	}

	std::map<int, common::XY<double>> Island::AllLocations()
	{
		std::map<int, common::XY<double>> result;
		auto records = Common::Execute(QUERY_ALL_LOCATIONS);
		std::transform(
			records.begin(), 
			records.end(), 
			std::inserter(result, result.end()), 
			[](const Common::Record& record) 
			{
				return std::make_pair(
					Common::ToInt(record, FIELD_ISLAND_ID),
					common::XY<double>(
						Common::ToDouble(record, FIELD_X),
						Common::ToDouble(record, FIELD_Y)));
			});
		return result;
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
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, islandId),
			FIELD_NAME);
	}

	std::optional<int> Island::ReadPatronDemigodId(int islandId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_PATRON_DEMIGOD_ID, islandId),
			FIELD_PATRON_DEMIGOD_ID);
	}
}