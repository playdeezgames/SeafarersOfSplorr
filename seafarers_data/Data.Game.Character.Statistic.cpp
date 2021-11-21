#include <Common.Data.h>
#include "Data.Game.Character.Statistic.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <string>
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterStatistics]([CharacterId] INT NOT NULL,[StatisticId] INT NOT NULL,[Minimum] REAL NULL,[Maximum] REAL NULL,[Current] REAL NOT NULL, UNIQUE([CharacterId],[StatisticId]));";
	static const std::string QUERY_ITEM = "SELECT [Minimum], [Maximum], [Current] FROM [CharacterStatistics] WHERE [StatisticId] = {} AND [CharacterId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterStatistics]([CharacterId],[StatisticId],[Minimum],[Maximum],[Current]) VALUES ({},{},{},{},{});";

	static const std::string FIELD_MINIMUM = "Minimum";
	static const std::string FIELD_MAXIMUM = "Maximum";
	static const std::string FIELD_CURRENT = "Current";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void Statistic::Write(int characterId, int statisticId, const Statistic& data)
	{
		AutoCreateTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			statisticId, 
			common::Data::OfOptional(data.minimum), 
			common::Data::OfOptional(data.maximum), 
			data.current);
	}

	static Statistic ToStatistic(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToOptionalDouble(record.find(FIELD_MINIMUM)->second),
			common::Data::ToOptionalDouble(record.find(FIELD_MAXIMUM)->second),
			common::Data::ToDouble(record.find(FIELD_CURRENT)->second)
		};
	}

	std::optional<Statistic> Statistic::Read(int characterId, int statisticId)
	{
		AutoCreateTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, statisticId, characterId);
		if (!records.empty())
		{
			return ToStatistic(records.front());
		}
		return std::nullopt;
	}
}