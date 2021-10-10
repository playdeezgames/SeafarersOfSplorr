#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Quest.h"
namespace data::game::island//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandQuests]([X] REAL NOT NULL, [Y] REAL NOT NULL, [DestinationX] REAL NOT NULL, [DestinationY] REAL NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL, [ReceiptEmotion] TEXT NOT NULL , [Reward] REAL NOT NULL, UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion] FROM [IslandQuests] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandQuests]([X],[Y],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion]) VALUES({:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{},{},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [IslandQuests] WHERE [X]={:.4f} AND  [Y]={:.4f};";

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_DESTINATION_X = "DestinationX";
	static const std::string FIELD_DESTINATION_Y = "DestinationY";
	static const std::string FIELD_REWARD = "Reward";
	static const std::string FIELD_PERSON_NAME = "PersonName";
	static const std::string FIELD_ITEM_NAME = "ItemName";
	static const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	static const std::string FIELD_RECEIPT_EMOTION = "ReceiptEmotion";

	static const auto AutoCreateIslandQuestsTable = data::game::Common::Run(CREATE_TABLE);

	void Quest::Write(const Quest& data)
	{
		AutoCreateIslandQuestsTable();
		data::game::Common::Execute(
			REPLACE_ITEM,
			data.location.GetX(),
			data.location.GetY(),
			data.destination.GetX(),
			data.destination.GetY(),
			data.reward,
			common::Data::QuoteString(data.itemName),
			common::Data::QuoteString(data.personName),
			common::Data::QuoteString(data.professionName),
			common::Data::QuoteString(data.receiptEmotion));
	}

	static Quest ToQuest(const std::map<std::string, std::string>& record)
	{
		return 
		{
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			{
				common::Data::ToDouble(record.find(FIELD_DESTINATION_X)->second),
				common::Data::ToDouble(record.find(FIELD_DESTINATION_Y)->second)
			},
			common::Data::ToDouble(record.find(FIELD_REWARD)->second),
			record.find(FIELD_ITEM_NAME)->second,
			record.find(FIELD_PERSON_NAME)->second,
			record.find(FIELD_PROFESSION_NAME)->second,
			record.find(FIELD_RECEIPT_EMOTION)->second
		};
	}

	std::optional<Quest> Quest::Read(const common::XY<double>& location)
	{
		AutoCreateIslandQuestsTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, location.GetX(), location.GetY());
		if (!records.empty())
		{
			return ToQuest(records.front());
		}
		return std::nullopt;
	}

	void Quest::Clear(const common::XY<double>& location)
	{
		AutoCreateIslandQuestsTable();
		data::game::Common::Execute(DELETE_ITEM, location.GetX(), location.GetY());
	}
}