#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishboardCell.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [FishboardCells]
		(
			[CharacterId] INT NOT NULL, 
			[X] INT NOT NULL, 
			[Y] INT NOT NULL, 
			[Revealed] INT NOT NULL, 
			[FishType] INT NULL, 
			UNIQUE([CharacterId],[X],[Y])
		);)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [FishboardCells] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [FishboardCells] 
		(
			[CharacterId],
			[X],
			[Y],
			[Revealed],
			[FishType]
		) 
		VALUES ({},{},{},{},{});)"s;
	static const std::string QUERY_ALL = 
		R"(SELECT 
			[X],
			[Y],
			[Revealed],
			[FishType] 
		FROM [FishboardCells] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[X],
			[Y],
			[Revealed],
			[FishType] 
		FROM [FishboardCells] 
		WHERE 
			[CharacterId]={} 
			AND [X]={} 
			AND [Y]={};)"s;
	static const std::string QUERY_FISH_COUNT = 
		R"(SELECT 
			COUNT([FishType]) AS [FishCount] 
		FROM [FishboardCells] 
		WHERE 
			[CharacterId]={} 
			AND [FishType] IS NOT NULL;)"s;
	static const std::string QUERY_REVEALED_FISH_COUNT = 
		R"(SELECT 
			COUNT([FishType]) AS [FishCount] 
		FROM [FishboardCells] 
		WHERE 
			[CharacterId]={} 
			AND [FishType] IS NOT NULL 
			AND [Revealed]<>0;)"s;
	static const std::string QUERY_FISHES = 
		R"(SELECT DISTINCT 
			[FishType] 
		FROM [FishboardCells] 
		WHERE 
			[CharacterId]={} 
			AND [FishType] IS NOT NULL;)"s;

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_REVEALED = "Revealed";
	static const std::string FIELD_FISH_TYPE = "FishType";
	static const std::string FIELD_FISH_COUNT = "FishCount";

	static const auto AutoCreateFishboardCellsTable = Common::Run(CREATE_TABLE);

	void FishboardCell::Clear(int characterId)
	{
		AutoCreateFishboardCellsTable();
		Common::Execute(DELETE_ALL, characterId);
	}

	void FishboardCell::Write(int characterId, const FishboardCell& cell)
	{
		AutoCreateFishboardCellsTable();
		Common::Execute(
			REPLACE_ITEM,
			characterId,
			cell.location.GetX(),
			cell.location.GetY(),
			(cell.revealed) ? (1) : (0),
			common::Data::OfOptional(cell.fishType));
	}

	static FishboardCell ToFishBoard(const std::map<std::string, std::string> record)
	{
		return {
			{
				common::Data::ToInt(record.find(FIELD_X)->second),
				common::Data::ToInt(record.find(FIELD_Y)->second)
			},
			common::Data::ToInt(record.find(FIELD_REVEALED)->second) > 0,
			common::Data::ToOptionalInt(record.find(FIELD_FISH_TYPE)->second)
		};
	}

	std::list<FishboardCell> FishboardCell::All(int characterId)
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(QUERY_ALL, characterId);
		std::list<FishboardCell> results;
		for (auto& record : records)
		{
			results.push_back(ToFishBoard(record));
		}
		return results;
	}

	std::optional<FishboardCell> FishboardCell::Read(int characterId, const common::XY<int>& location)
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, location.GetX(), location.GetY());
		if (!records.empty())
		{
			return ToFishBoard(records.front());
		}
		return std::nullopt;
	}

	size_t FishboardCell::ReadFishCount(int characterId)
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(QUERY_FISH_COUNT, characterId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front().find(FIELD_FISH_COUNT)->second);
		}
		return 0;
	}

	size_t FishboardCell::ReadRevealedFishCount(int characterId)
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(QUERY_REVEALED_FISH_COUNT, characterId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front().find(FIELD_FISH_COUNT)->second);
		}
		return 0;
	}

	std::set<int> FishboardCell::ReadFish(int characterId)
	{
		std::set<int> result;
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(QUERY_FISHES, characterId);
		for (auto cell : records)
		{
			result.insert(common::Data::ToInt(cell.find(FIELD_FISH_TYPE)->second));
		}
		return result;
	}
}