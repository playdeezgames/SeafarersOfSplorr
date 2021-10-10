#include <Common.Data.h>
#include "Data.Game.Ship.Statistic.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <string>
namespace data::game::ship//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [ShipStatistics]([ShipId] INT NOT NULL, [StatisticId] INT NOT NULL,[Minimum] REAL NULL,[Maximum] REAL NULL,[Current] REAL NOT NULL, UNIQUE([ShipId],[StatisticId]));";
	static const std::string QUERY_ITEM = "SELECT [Minimum], [Maximum], [Current] FROM [ShipStatistics] WHERE [StatisticId] = {} AND [ShipId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [ShipStatistics]([ShipId],[StatisticId],[Minimum],[Maximum],[Current]) VALUES ({},{},{},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [ShipStatistics] WHERE [ShipId]={};";

	static const std::string FIELD_MINIMUM = "Minimum";
	static const std::string FIELD_MAXIMUM = "Maximum";
	static const std::string FIELD_CURRENT = "Current";

	static const auto AutoCreateShipStatisticsTable = data::game::Common::Run(CREATE_TABLE);

	void Statistic::Write(int shipId, int statisticId, const Statistic& data)
	{
		AutoCreateShipStatisticsTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			shipId, 
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

	std::optional<Statistic> Statistic::Read(int shipId, int statisticId)
	{
		AutoCreateShipStatisticsTable();
		auto result = data::game::Common::Execute(QUERY_ITEM, statisticId, shipId);
		if (!result.empty())
		{
			return ToStatistic(result.front());
		}
		return std::nullopt;
	}

	void Statistic::Clear(int shipId)
	{
		AutoCreateShipStatisticsTable();
		data::game::Common::Execute(
			DELETE_ALL, 
			shipId);
	}

}