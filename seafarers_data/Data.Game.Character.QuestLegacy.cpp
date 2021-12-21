#include <Common.Data.h>
#include "Data.Game.Character.QuestLegacy.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE =
		"CREATE TABLE IF NOT EXISTS [CharacterQuests]"
		"("
			"[CharacterId] INT NOT NULL UNIQUE,"
			"[ToIslandId] INT NOT NULL, "
			"[ItemName] TEXT NOT NULL, "
			"[PersonName] TEXT NOT NULL, "
			"[ProfessionName] TEXT NOT NULL, "
			"[ReceiptEmotion] TEXT NOT NULL, "
			"[Reward] REAL NOT NULL"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[ToIslandId], "
			"[Reward], "
			"[ItemName], "
			"[PersonName], "
			"[ProfessionName], "
			"[ReceiptEmotion] "
		"FROM [CharacterQuests] "
		"WHERE "
			"[CharacterId] = {};";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterQuests]"
		"("
			"[CharacterId],"
			"[ToIslandId],"
			"[Reward],"
			"[ItemName],"
			"[PersonName],"
			"[ProfessionName],"
			"[ReceiptEmotion]"
		") "
		"VALUES({},{},{},{},{},{},{});";
	static const std::string DELETE_ITEM = 
		"DELETE FROM [CharacterQuests] "
		"WHERE "
			"[CharacterId]={};";

	static const std::string FIELD_TO_ISLAND_ID = "ToIslandId";
	static const std::string FIELD_REWARD = "Reward";
	static const std::string FIELD_PERSON_NAME = "PersonName";
	static const std::string FIELD_ITEM_NAME = "ItemName";
	static const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	static const std::string FIELD_RECEIPT_EMOTION = "ReceiptEmotion";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void QuestLegacy::Write(int characterId, const std::optional<QuestLegacy>& data)
	{
		AutoCreateTable();
		data::game::Common::Execute(DELETE_ITEM, characterId);
		if (data)
		{
			data::game::Common::Execute(
				REPLACE_ITEM,
				characterId,
				data.value().toIslandId,
				data.value().reward,
				common::Data::QuoteString(data.value().itemName),
				common::Data::QuoteString(data.value().personName),
				common::Data::QuoteString(data.value().professionName),
				common::Data::QuoteString(data.value().receiptEmotion));
		}
	}

	static QuestLegacy ToQuest(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_TO_ISLAND_ID)->second),
			common::Data::ToDouble(record.find(FIELD_REWARD)->second),
			record.find(FIELD_ITEM_NAME)->second,
			record.find(FIELD_PERSON_NAME)->second,
			record.find(FIELD_PROFESSION_NAME)->second,
			record.find(FIELD_RECEIPT_EMOTION)->second
		};
	}

	std::optional<QuestLegacy> QuestLegacy::Read(int characterId)
	{
		AutoCreateTable();
		auto records = data::game::Common::Execute(
			QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return ToQuest(records.front());
		}
		return std::nullopt;
	}
}