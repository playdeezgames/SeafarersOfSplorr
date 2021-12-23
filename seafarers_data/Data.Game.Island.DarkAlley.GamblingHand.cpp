#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.GamblingHand.h"
namespace data::game::island::dark_alley
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [DarkAlleyGamblingHands]
		(
			[IslandId] INT NOT NULL,
			[FirstCard] INT NOT NULL,
			[SecondCard] INT NOT NULL,
			[ThirdCard] INT NOT NULL, 
			UNIQUE([IslandId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [DarkAlleyGamblingHands]
		(
			[IslandId],
			[FirstCard],
			[SecondCard],
			[ThirdCard]
		) 
		VALUES ({},{},{},{});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[FirstCard],
			[SecondCard],
			[ThirdCard] 
		FROM [DarkAlleyGamblingHands] 
		WHERE 
			[IslandId]={};)"s;

	static const std::string FIELD_FIRST_CARD = "FirstCard";
	static const std::string FIELD_SECOND_CARD = "SecondCard";
	static const std::string FIELD_THIRD_CARD = "ThirdCard";

	static const auto AutoCreateDarkAlleyGamblingHandsTable = data::game::Common::Run(CREATE_TABLE);

	void GamblingHand::Write(int islandId, const GamblingHand& data)
	{
		AutoCreateDarkAlleyGamblingHandsTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			islandId, 
			data.firstCard, 
			data.secondCard, 
			data.thirdCard);
	}

	static GamblingHand ToGamblingHand(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_FIRST_CARD)->second),
			common::Data::ToInt(record.find(FIELD_SECOND_CARD)->second),
			common::Data::ToInt(record.find(FIELD_THIRD_CARD)->second)
		};
	}

	std::optional<GamblingHand> GamblingHand::Read(int islandId)
	{
		AutoCreateDarkAlleyGamblingHandsTable();
		auto records = Common::Execute(QUERY_ITEM, islandId);
		if (!records.empty())
		{
			return ToGamblingHand(records.front());
		}
		return std::nullopt;
	}
}