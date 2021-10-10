#include <Common.Data.h>
#include "Data.Game.Avatar.Quest.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
namespace data::game::avatar//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarQuests]([AvatarId] INT NOT NULL UNIQUE,[DestinationX] REAL NOT NULL, [DestinationY] REAL NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL, [ReceiptEmotion] TEXT NOT NULL, [Reward] REAL NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion] FROM [AvatarQuests] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarQuests]([AvatarId],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion]) VALUES({},{:.4f},{:.4f},{:.4f},{},{},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarQuests] WHERE [AvatarId]={};";

	static const std::string FIELD_DESTINATION_X = "DestinationX";
	static const std::string FIELD_DESTINATION_Y = "DestinationY";
	static const std::string FIELD_REWARD = "Reward";
	static const std::string FIELD_PERSON_NAME = "PersonName";
	static const std::string FIELD_ITEM_NAME = "ItemName";
	static const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	static const std::string FIELD_RECEIPT_EMOTION = "ReceiptEmotion";

	static const auto AutoCreateAvatarQuestTable = data::game::Common::Run(CREATE_TABLE);

	void Quest::Write(int avatarId, const std::optional<Quest>& data)
	{
		AutoCreateAvatarQuestTable();
		data::game::Common::Execute(DELETE_ITEM, avatarId);
		if (data)
		{
			data::game::Common::Execute(
				REPLACE_ITEM,
				data::game::Player::GetAvatarId(),
				data.value().destination.GetX(),
				data.value().destination.GetY(),
				data.value().reward,
				common::Data::QuoteString(data.value().itemName),
				common::Data::QuoteString(data.value().personName),
				common::Data::QuoteString(data.value().professionName),
				common::Data::QuoteString(data.value().receiptEmotion));
		}
	}

	static Quest ToQuest(const std::map<std::string, std::string>& record)
	{
		return
		{
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

	std::optional<Quest> Quest::Read(int avatarId)
	{
		AutoCreateAvatarQuestTable();
		auto records = data::game::Common::Execute(
			QUERY_ITEM, avatarId);
		if (!records.empty())
		{
			return ToQuest(records.front());
		}
		return std::nullopt;
	}
}