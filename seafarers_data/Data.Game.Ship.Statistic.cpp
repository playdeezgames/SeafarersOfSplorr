#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Ship.Statistic.h"
namespace data::game::ship
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ShipStatistics]
		(
			[ShipId] INT NOT NULL, 
			[StatisticId] INT NOT NULL,
			[Minimum] REAL NULL,
			[Maximum] REAL NULL,
			[Current] REAL NOT NULL, 
			UNIQUE([ShipId],[StatisticId])
		);)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Minimum], 
			[Maximum], 
			[Current] 
		FROM [ShipStatistics] 
		WHERE 
			[StatisticId] = {} 
			AND [ShipId]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [ShipStatistics]
		(
			[ShipId],
			[StatisticId],
			[Minimum],
			[Maximum],
			[Current]
		) 
		VALUES ({},{},{},{},{});)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [ShipStatistics] 
		WHERE 
			[ShipId]={};)"s;

	static const std::string FIELD_MINIMUM = "Minimum";
	static const std::string FIELD_MAXIMUM = "Maximum";
	static const std::string FIELD_CURRENT = "Current";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void Statistic::Write(int shipId, int statisticId, const Statistic& data)
	{
		AutoCreateTable();
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
		AutoCreateTable();
		auto result = data::game::Common::Execute(QUERY_ITEM, statisticId, shipId);
		if (!result.empty())
		{
			return ToStatistic(result.front());
		}
		return std::nullopt;
	}

	void Statistic::Clear(int shipId)
	{
		AutoCreateTable();
		data::game::Common::Execute(
			DELETE_ALL, 
			shipId);
	}

}