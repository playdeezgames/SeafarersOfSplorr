#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.QuestLegacy.h"
namespace data::game::island
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandQuests]([FromIslandId] INT NOT NULL, [ToIslandId] INT NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL, [ReceiptEmotion] TEXT NOT NULL , [Reward] REAL NOT NULL, UNIQUE([FromIslandId]));";
	static const std::string QUERY_ITEM = "SELECT [FromIslandId],[ToIslandId],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion] FROM [IslandQuests] WHERE [FromIslandId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandQuests]([FromIslandId],[ToIslandId],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion]) VALUES({},{},{:.4f},{},{},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [IslandQuests] WHERE [FromIslandId]={};";

	static const std::string FIELD_FROM_ISLAND_ID = "FromIslandId";
	static const std::string FIELD_TO_ISLAND_ID = "ToIslandId";
	static const std::string FIELD_REWARD = "Reward";
	static const std::string FIELD_PERSON_NAME = "PersonName";
	static const std::string FIELD_ITEM_NAME = "ItemName";
	static const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	static const std::string FIELD_RECEIPT_EMOTION = "ReceiptEmotion";

	static const auto AutoCreateIslandQuestsTable = data::game::Common::Run(CREATE_TABLE);

	void QuestLegacy::Write(const QuestLegacy& data)
	{
		AutoCreateIslandQuestsTable();
		data::game::Common::Execute(
			REPLACE_ITEM,
			data.fromIslandId,
			data.toIslandId,
			data.reward,
			common::Data::QuoteString(data.itemName),
			common::Data::QuoteString(data.personName),
			common::Data::QuoteString(data.professionName),
			common::Data::QuoteString(data.receiptEmotion));
	}

	static QuestLegacy ToQuest(const std::map<std::string, std::string>& record)
	{
		return 
		{
			common::Data::ToInt(record.find(FIELD_FROM_ISLAND_ID)->second),
			common::Data::ToInt(record.find(FIELD_TO_ISLAND_ID)->second),
			common::Data::ToDouble(record.find(FIELD_REWARD)->second),
			record.find(FIELD_ITEM_NAME)->second,
			record.find(FIELD_PERSON_NAME)->second,
			record.find(FIELD_PROFESSION_NAME)->second,
			record.find(FIELD_RECEIPT_EMOTION)->second
		};
	}

	std::optional<QuestLegacy> QuestLegacy::Read(int islandId)
	{
		AutoCreateIslandQuestsTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, islandId);
		if (!records.empty())
		{
			return ToQuest(records.front());
		}
		return std::nullopt;
	}

	void QuestLegacy::Clear(int islandId)
	{
		AutoCreateIslandQuestsTable();
		data::game::Common::Execute(DELETE_ITEM, islandId);
	}
}