#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.GamblingHand.h"
namespace data::game::island::dark_alley//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DarkAlleyGamblingHands]([X] REAL NOT NULL,[Y] REAL NOT NULL,[FirstCard] INT NOT NULL,[SecondCard] INT NOT NULL,[ThirdCard] INT NOT NULL, UNIQUE([X],[Y]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DarkAlleyGamblingHands]([X],[Y],[FirstCard],[SecondCard],[ThirdCard]) VALUES ({:.4f},{:.4f},{},{},{});";
	static const std::string QUERY_ITEM = "SELECT [FirstCard],[SecondCard],[ThirdCard] FROM [DarkAlleyGamblingHands] WHERE [X]={:.4f} AND [Y]={:.4f};";

	static const std::string FIELD_FIRST_CARD = "FirstCard";
	static const std::string FIELD_SECOND_CARD = "SecondCard";
	static const std::string FIELD_THIRD_CARD = "ThirdCard";

	static const auto AutoCreateDarkAlleyGamblingHandsTable = data::game::Common::Run(CREATE_TABLE);

	void GamblingHand::Write(const common::XY<double>& location, const GamblingHand& data)
	{
		AutoCreateDarkAlleyGamblingHandsTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			location.GetX(), 
			location.GetY(), 
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

	std::optional<GamblingHand> GamblingHand::Read(const common::XY<double>& location)
	{
		AutoCreateDarkAlleyGamblingHandsTable();
		auto records = Common::Execute(QUERY_ITEM, location.GetX(), location.GetY());
		if (!records.empty())
		{
			return ToGamblingHand(records.front());
		}
		return std::nullopt;
	}
}