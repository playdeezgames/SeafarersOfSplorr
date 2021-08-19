#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Quests.h"
#include <format>
namespace data::game::island::Quests
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_DESTINATION_X = "DestinationX";
	const std::string FIELD_DESTINATION_Y = "DestinationY";
	const std::string FIELD_REWARD = "Reward";
	const std::string FIELD_PERSON_NAME = "PersonName";
	const std::string FIELD_ITEM_NAME = "ItemName";
	const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandQuests]([X] REAL NOT NULL, [Y] REAL NOT NULL, [DestinationX] REAL NOT NULL, [DestinationY] REAL NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL , [Reward] REAL NOT NULL, UNIQUE([X],[Y]));";
	const std::string QUERY_ITEM = "SELECT [X],[Y],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName] FROM [IslandQuests] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	const std::string REPLACE_ITEM = "REPLACE INTO [IslandQuests]([X],[Y],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName]) VALUES({:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{},{},{});";
	const std::string DELETE_ITEM = "DELETE FROM [IslandQuests] WHERE [X]={:.4f} AND  [Y]={:.4f};";

	const auto AutoCreateIslandQuestsTable = data::game::Common::Run(CREATE_TABLE);

	void Write(const QuestData& data)
	{
		AutoCreateIslandQuestsTable();
		std::string query =
			std::format(
				REPLACE_ITEM,
				data.location.GetX(),
				data.location.GetY(),
				data.destination.GetX(),
				data.destination.GetY(),
				data.reward,
				common::Data::QuoteString(data.itemName),
				common::Data::QuoteString(data.personName),
				common::Data::QuoteString(data.professionName));
		data::game::Common::Execute(query);
	}

	std::optional<QuestData> Read(const common::XY<double>& location)
	{
		AutoCreateIslandQuestsTable();
		std::string query = std::format(QUERY_ITEM, location.GetX(), location.GetY());
		auto records = data::game::Common::Execute(query);
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<QuestData>(
				{
					{
						common::Data::ToDouble(record[FIELD_X]),
						common::Data::ToDouble(record[FIELD_Y])
					},
					{
						common::Data::ToDouble(record[FIELD_DESTINATION_X]),
						common::Data::ToDouble(record[FIELD_DESTINATION_Y])
					},
					common::Data::ToDouble(record[FIELD_REWARD]),
					record[FIELD_ITEM_NAME],
					record[FIELD_PERSON_NAME],
					record[FIELD_PROFESSION_NAME]
				});
		}
		return std::nullopt;
	}

	void Clear(const common::XY<double>& location)
	{
		AutoCreateIslandQuestsTable();
		std::string query = std::format(DELETE_ITEM, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}
}