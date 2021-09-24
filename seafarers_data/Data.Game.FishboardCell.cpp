#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishboardCell.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [FishboardCells]([AvatarId] INT NOT NULL, [X] INT NOT NULL, [Y] INT NOT NULL, [Revealed] INT NOT NULL, [FishType] INT NULL, UNIQUE([AvatarId],[X],[Y]));";
	static const std::string DELETE_ALL = "DELETE FROM [FishboardCells] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [FishboardCells] ([AvatarId],[X],[Y],[Revealed],[FishType]) VALUES ({},{},{},{},{});";
	static const std::string QUERY_ALL = "SELECT [X],[Y],[Revealed],[FishType] FROM [FishboardCells] WHERE [AvatarId]={};";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[Revealed],[FishType] FROM [FishboardCells] WHERE [AvatarId]={} AND [X]={} AND [Y]={};";
	static const std::string QUERY_FISH_COUNT = "SELECT COUNT([FishType]) AS [FishCount] FROM [FishboardCells] WHERE [AvatarId]={} AND [FishType] IS NOT NULL;";
	static const std::string QUERY_REVEALED_FISH_COUNT = "SELECT COUNT([FishType]) AS [FishCount] FROM [FishboardCells] WHERE [AvatarId]={} AND [FishType] IS NOT NULL AND [Revealed]<>0;";
	static const std::string QUERY_FISHES = "SELECT DISTINCT [FishType] FROM [FishboardCells] WHERE [AvatarId]={} AND [FishType] IS NOT NULL;";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_REVEALED = "Revealed";
	static const std::string FIELD_FISH_TYPE = "FishType";
	static const std::string FIELD_FISH_COUNT = "FishCount";

	static const auto AutoCreateFishboardCellsTable = Common::Run(CREATE_TABLE);

	void FishboardCell::Clear()
	{
		AutoCreateFishboardCellsTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}

	void FishboardCell::Write(const FishboardCell& cell)
	{
		AutoCreateFishboardCellsTable();
		Common::Execute(
			std::format(
				REPLACE_ITEM,
				Common::AVATAR_ID,
				cell.location.GetX(),
				cell.location.GetY(),
				(cell.revealed) ? (1) : (0),
				common::Data::OfOptional(cell.fishType)));
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

	std::list<FishboardCell> FishboardCell::All()
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(std::format(QUERY_ALL, Common::AVATAR_ID));
		std::list<FishboardCell> results;
		for (auto& record : records)
		{
			results.push_back(ToFishBoard(record));
		}
		return results;
	}

	std::optional<FishboardCell> FishboardCell::Read(const common::XY<int>& location)
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, location.GetX(), location.GetY()));
		if (!records.empty())
		{
			return ToFishBoard(records.front());
		}
		return std::nullopt;
	}

	size_t FishboardCell::ReadFishCount()
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(std::format(QUERY_FISH_COUNT, Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::ToInt(records.front().find(FIELD_FISH_COUNT)->second);
		}
		return 0;
	}

	size_t FishboardCell::ReadRevealedFishCount()
	{
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(std::format(QUERY_REVEALED_FISH_COUNT, Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::ToInt(records.front().find(FIELD_FISH_COUNT)->second);
		}
		return 0;
	}

	std::set<int> FishboardCell::ReadFish()
	{
		std::set<int> result;
		AutoCreateFishboardCellsTable();
		auto records = Common::Execute(std::format(QUERY_FISHES, Common::AVATAR_ID));
		for (auto cell : records)
		{
			result.insert(common::Data::ToInt(cell.find(FIELD_FISH_TYPE)->second));
		}
		return result;
	}
}