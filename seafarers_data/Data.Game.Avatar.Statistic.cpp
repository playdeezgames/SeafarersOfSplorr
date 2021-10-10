#include <Common.Data.h>
#include "Data.Game.Avatar.Statistic.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <string>
namespace data::game::avatar//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarStatistics]([AvatarId] INT NOT NULL,[StatisticId] INT NOT NULL,[Minimum] REAL NULL,[Maximum] REAL NULL,[Current] REAL NOT NULL, UNIQUE([AvatarId],[StatisticId]));";
	static const std::string QUERY_ITEM = "SELECT [Minimum], [Maximum], [Current] FROM [AvatarStatistics] WHERE [StatisticId] = {} AND [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarStatistics]([AvatarId],[StatisticId],[Minimum],[Maximum],[Current]) VALUES ({},{},{},{},{});";

	static const std::string FIELD_MINIMUM = "Minimum";
	static const std::string FIELD_MAXIMUM = "Maximum";
	static const std::string FIELD_CURRENT = "Current";

	static const auto AutoCreateAvatarStatisticsTable = data::game::Common::Run(CREATE_TABLE);

	void Statistic::Write(int avatarId, int statisticId, const Statistic& data)
	{
		AutoCreateAvatarStatisticsTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			avatarId, 
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

	std::optional<Statistic> Statistic::Read(int avatarId, int statisticId)
	{
		AutoCreateAvatarStatisticsTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, statisticId, avatarId);
		if (!records.empty())
		{
			return ToStatistic(records.front());
		}
		return std::nullopt;
	}
}