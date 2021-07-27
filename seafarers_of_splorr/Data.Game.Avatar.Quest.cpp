#include "Common.Data.h"
#include "Data.Game.Avatar.Quest.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Quest
{
	const std::string FIELD_DESTINATION_X = "DestinationX";
	const std::string FIELD_DESTINATION_Y = "DestinationY";
	const std::string FIELD_REWARD = "Reward";
	const std::string FIELD_PERSON_NAME = "PersonName";
	const std::string FIELD_ITEM_NAME = "ItemName";
	const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarQuests]([AvatarId] INT NOT NULL UNIQUE,[DestinationX] REAL NOT NULL, [DestinationY] REAL NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL , [Reward] REAL NOT NULL);";
	const std::string QUERY_ITEM = "SELECT [DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName] FROM [AvatarQuests] WHERE [AvatarId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarQuests]([AvatarId],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName]) VALUES({},{:.4f},{:.4f},{:.4f},{},{},{});";
	const std::string DELETE_ITEM = "DELETE FROM [AvatarQuests] WHERE [AvatarId]={};";

	static void AutoCreateAvatarQuestTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(const std::optional<QuestData>& data)
	{
		AutoCreateAvatarQuestTable();
		if (data)
		{
			data::game::Common::Execute(
				std::format(REPLACE_ITEM,
					data::game::Common::AVATAR_ID,
					data.value().destination.GetX(),
					data.value().destination.GetY(),
					data.value().reward,
					data::game::Common::QuoteString(data.value().itemName),
					data::game::Common::QuoteString(data.value().personName),
					data::game::Common::QuoteString(data.value().professionName)));
		}
		else
		{
			data::game::Common::Execute(std::format(DELETE_ITEM, data::game::Common::AVATAR_ID));
		}
	}

	std::optional<QuestData> Read()
	{
		AutoCreateAvatarQuestTable();
		auto records = data::game::Common::Execute(
			std::format(QUERY_ITEM, data::game::Common::AVATAR_ID));
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<QuestData>({
					{common::Data::StringToDouble(record[FIELD_DESTINATION_X]),common::Data::StringToDouble(record[FIELD_DESTINATION_Y])},
					common::Data::StringToDouble(record[FIELD_REWARD]),
					record[FIELD_ITEM_NAME],
					record[FIELD_PERSON_NAME],
					record[FIELD_PROFESSION_NAME]});
		}
		return std::nullopt;
	}
}