#include <Common.Data.h>
#include "Data.Game.Avatar.ShipStatistic.h"
#include "Data.Game.Common.h"
#include <format>
#include <string>
namespace data::game::avatar
{
	static const std::string FIELD_MINIMUM = "Minimum";
	static const std::string FIELD_MAXIMUM = "Maximum";
	static const std::string FIELD_CURRENT = "Current";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarShipStatistics]([AvatarId] INT NOT NULL, [StatisticId] INT NOT NULL,[Minimum] REAL NULL,[Maximum] REAL NULL,[Current] REAL NOT NULL, UNIQUE([AvatarId],[StatisticId]));";
	static const std::string QUERY_ITEM = "SELECT [Minimum], [Maximum], [Current] FROM [AvatarShipStatistics] WHERE [StatisticId] = {} AND [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarShipStatistics]([AvatarId],[StatisticId],[Minimum],[Maximum],[Current]) VALUES ({},{},{},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarShipStatistics] WHERE [AvatarId]={};";

	static const auto AutoCreateAvatarShipStatisticsTable = data::game::Common::Run(CREATE_TABLE);

	void ShipStatistic::Write(int avatarId, int statisticId, const ShipStatistic& data)
	{
		AutoCreateAvatarShipStatisticsTable();
		auto query = std::format(REPLACE_ITEM, avatarId, statisticId, common::Data::OfOptional(data.minimum), common::Data::OfOptional(data.maximum), data.current);
		data::game::Common::Execute(query);
	}

	std::optional<ShipStatistic> ShipStatistic::Read(int avatarId, int statisticId)
	{
		AutoCreateAvatarShipStatisticsTable();
		auto query = std::format(QUERY_ITEM, statisticId, avatarId);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto record = result.front();
			return std::optional<ShipStatistic>(
				{
					common::Data::ToOptionalDouble(record[FIELD_MINIMUM]),
					common::Data::ToOptionalDouble(record[FIELD_MAXIMUM]),
					common::Data::ToDouble(record[FIELD_CURRENT])
				});
		}
		return std::nullopt;
	}

	void ShipStatistic::Clear(int avatarId)
	{
		AutoCreateAvatarShipStatisticsTable();
		data::game::Common::Execute(std::format(DELETE_ALL, avatarId));
	}

	void ShipStatistic::Write(int statisticId, const ShipStatistic& statistic)
	{
		Write(Common::AVATAR_ID, statisticId, statistic);
	}

	std::optional<ShipStatistic> ShipStatistic::Read(int statisticId)
	{
		return Read(Common::AVATAR_ID, statisticId);
	}

	void ShipStatistic::Clear()
	{
		Clear(Common::AVATAR_ID);
	}
}