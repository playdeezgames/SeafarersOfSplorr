#include "Common.Data.h"
#include "Data.Game.Avatar.Statistics.h"
#include "Data.Game.Common.h"
#include <format>
#include <string>
namespace data::game::avatar::Statistics
{
	const std::string FIELD_MINIMUM = "Minimum";
	const std::string FIELD_MAXIMUM = "Maximum";
	const std::string FIELD_CURRENT = "Current";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarStatistics]([StatisticId] INT NOT NULL UNIQUE,[Minimum] REAL NULL,[Maximum] REAL NULL,[Current] REAL NOT NULL);";
	const std::string QUERY_ITEM = "SELECT [Minimum], [Maximum], [Current] FROM [AvatarStatistics] WHERE [StatisticId] = {};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarStatistics]([StatisticId],[Minimum],[Maximum],[Current]) VALUES ({},{},{},{});";

	const auto AutoCreateAvatarStatisticsTable = data::game::Common::Run(CREATE_TABLE);

	void Write(int statisticId, const StatisticData& data)
	{
		AutoCreateAvatarStatisticsTable();
		auto query = std::format(REPLACE_ITEM, statisticId, Common::OfOptional(data.minimum), Common::OfOptional(data.maximum), data.current);
		data::game::Common::Execute(query);
	}

	std::optional<StatisticData> Read(int statisticId)
	{
		AutoCreateAvatarStatisticsTable();
		auto query = std::format(QUERY_ITEM, statisticId);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto record = result.front();
			return std::optional<StatisticData>(
			{
				common::Data::ToOptionalDouble(record[FIELD_MINIMUM]),
				common::Data::ToOptionalDouble(record[FIELD_MAXIMUM]),
				common::Data::ToDouble(record[FIELD_CURRENT])
			});
		}
		return std::nullopt;
	}

}