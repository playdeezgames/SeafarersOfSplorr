#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishboardCell.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [FishBoards]([AvatarId] INT NOT NULL, [X] INT NOT NULL, [Y] INT NOT NULL, [Revealed] INT NOT NULL, [FishType] INT NULL, UNIQUE([AvatarId],[X],[Y]));";
	static const std::string DELETE_ALL = "DELETE FROM [FishBoards] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [FishBoards] ([AvatarId],[X],[Y],[Revealed],[FishType]) VALUES ({},{},{},{},{});";
	static const std::string QUERY_ALL = "SELECT [X],[Y],[Revealed],[FishType] FROM [FishBoards] WHERE [AvatarId]={};";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[Revealed],[FishType] FROM [FishBoards] WHERE [AvatarId]={} AND [X]={} AND [Y]={};";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_REVEALED = "Revealed";
	static const std::string FIELD_FISH_TYPE = "FishType";

	static const auto AutoCreateFishBoardTable = Common::Run(CREATE_TABLE);

	void FishboardCell::Clear()
	{
		AutoCreateFishBoardTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}

	void FishboardCell::Write(const FishboardCell& cell)
	{
		AutoCreateFishBoardTable();
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
		AutoCreateFishBoardTable();
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
		AutoCreateFishBoardTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, location.GetX(), location.GetY()));
		if (!records.empty())
		{
			return ToFishBoard(records.front());
		}
		return std::nullopt;
	}

	size_t FishboardCell::ReadFishCount()
	{
		size_t count = 0;
		for (auto cell : All())
		{
			if (cell.fishType)
			{
				count++;
			}
		}
		return count;
	}

	size_t FishboardCell::ReadRevealedFishCount()
	{
		size_t count = 0;
		for (auto cell : All())
		{
			if (cell.fishType && cell.revealed)
			{
				count++;
			}
		}
		return count;
	}

	std::set<int> FishboardCell::ReadFish()
	{
		std::set<int> result;
		for (auto cell : All())
		{
			if (cell.fishType && !result.contains(cell.fishType.value()))
			{
				result.insert(cell.fishType.value());
			}
		}
		return result;
	}

}