#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.FightCard.h"
namespace data::game::island::dark_alley
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [DarkAlleyFightCards]
		(
			[IslandId] INT NOT NULL,
			[CardId] INT NOT NULL,
			[RankId] INT NOT NULL,
			[SuitId] INT NOT NULL,
			[Adjacent] INT NOT NULL,
			[Shown] INT NOT NULL,
			UNIQUE([IslandId], [CardId])
		);)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[RankId],
			[SuitId],
			[Adjacent],
			[Shown] 
		FROM [DarkAlleyFightCards] 
		WHERE 
			[IslandId]={} 
			AND [CardId]={};)"s;
	static const std::string QUERY_ITEMS = 
		R"(SELECT 
			[RankId],
			[SuitId],
			[Adjacent],
			[Shown],
			[CardId] 
		FROM [DarkAlleyFightCards] 
		WHERE 
			[IslandId]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [DarkAlleyFightCards]
		(
			[IslandId],
			[CardId],
			[RankId],
			[SuitId],
			[Adjacent],
			[Shown]
		) 
		VALUES({},{},{},{},{},{});)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [DarkAlleyFightCards] 
		WHERE 
			[IslandId]={};)"s;

	static const std::string FIELD_SUIT_ID = "SuitId";
	static const std::string FIELD_RANK_ID = "RankId";
	static const std::string FIELD_ADJACENT = "Adjacent";
	static const std::string FIELD_SHOWN = "Shown";
	static const std::string FIELD_CARD_ID = "CardId";

	static const auto AutoCreateDarkAlleyFightCardsTable = Common::Run(CREATE_TABLE);

	static FightCard ToFightCard(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_RANK_ID)->second),
			common::Data::ToInt(record.find(FIELD_SUIT_ID)->second),
			common::Data::ToInt(record.find(FIELD_ADJACENT)->second),
			common::Data::ToInt(record.find(FIELD_SHOWN)->second) > 0,
		};
	}

	std::optional<FightCard> FightCard::Read(int islandId, size_t index)
	{
		AutoCreateDarkAlleyFightCardsTable();
		auto records = Common::Execute(QUERY_ITEM, islandId, index);
		if (!records.empty())
		{
			return ToFightCard(records.front());
		}
		return std::nullopt;
	}

	void FightCard::Write(int islandId, size_t index, const FightCard& data)
	{
		AutoCreateDarkAlleyFightCardsTable();
		Common::Execute(
			REPLACE_ITEM, 
			islandId, 
			index, 
			data.rank, 
			data.suit, 
			data.adjacent, 
			data.shown ? 1 : 0);
	}

	void FightCard::Clear(int islandId)
	{
		AutoCreateDarkAlleyFightCardsTable();
		Common::Execute(DELETE_ALL, islandId);
	}

	std::map<size_t, FightCard> FightCard::Read(int islandId)
	{
		std::map<size_t, FightCard> result;
		AutoCreateDarkAlleyFightCardsTable();
		auto records = Common::Execute(QUERY_ITEMS, islandId);
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_CARD_ID])] =
				ToFightCard(record);
		}
		return result;

	}

}