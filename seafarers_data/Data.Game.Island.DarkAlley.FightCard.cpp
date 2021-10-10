#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.FightCard.h"
namespace data::game::island::dark_alley//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DarkAlleyFightCards]([X] REAL NOT NULL,[Y] REAL NOT NULL,[CardId] INT NOT NULL,[RankId] INT NOT NULL,[SuitId] INT NOT NULL,[Adjacent] INT NOT NULL,[Shown] INT NOT NULL,UNIQUE([X],[Y], [CardId]));";
	static const std::string QUERY_ITEM = "SELECT [RankId],[SuitId],[Adjacent],[Shown] FROM [DarkAlleyFightCards] WHERE [X]={:.4f} AND [Y]={:.4f} AND [CardId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DarkAlleyFightCards]([X],[Y],[CardId],[RankId],[SuitId],[Adjacent],[Shown]) VALUES({:.4f},{:.4f},{},{},{},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [DarkAlleyFightCards] WHERE [X]={:.4f} AND [Y]={:.4f};";

	static const std::string FIELD_SUIT_ID = "SuitId";
	static const std::string FIELD_RANK_ID = "RankId";
	static const std::string FIELD_ADJACENT = "Adjacent";
	static const std::string FIELD_SHOWN = "Shown";

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

	std::optional<FightCard> FightCard::Read(const common::XY<double>& location, size_t index)
	{
		AutoCreateDarkAlleyFightCardsTable();
		auto records = Common::Execute(QUERY_ITEM, location.GetX(), location.GetY(), index);
		if (!records.empty())
		{
			return ToFightCard(records.front());
		}
		return std::nullopt;
	}

	void FightCard::Write(const common::XY<double>& location, size_t index, const FightCard& data)
	{
		AutoCreateDarkAlleyFightCardsTable();
		Common::Execute(
			REPLACE_ITEM, 
			location.GetX(), 
			location.GetY(), 
			index, 
			data.rank, 
			data.suit, 
			data.adjacent, 
			data.shown ? 1 : 0);
	}

	void FightCard::Clear(const common::XY<double>& location)
	{
		AutoCreateDarkAlleyFightCardsTable();
		Common::Execute(DELETE_ALL, location.GetX(), location.GetY());
	}
}