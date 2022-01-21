#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Feature.h"
namespace data::game::island
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Features]
		(
			[FeatureId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[IslandId] INT NOT NULL,
			[Name] TEXT NOT NULL,
			[FeatureType] INT NOT NULL,
			[District] INT NOT NULL,
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"sv;
	static constexpr std::string_view INSERT_ITEM =
		R"(INSERT INTO [Features]
		(
			[IslandId],
			[Name],
			[FeatureType],
			[District]
		) 
		VALUES({},{},{},{});)"sv;
	static constexpr std::string_view QUERY_FOR_ISLAND_DISTRICT =
		R"(SELECT 
			[FeatureId] 
		FROM [Features] 
		WHERE 
			[IslandId]={} 
			AND [District]={};)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Features] 
		WHERE 
			[FeatureId]={};)"sv;

	static constexpr std::string_view FIELD_FEATURE_ID = "FeatureId";
	static constexpr std::string_view FIELD_NAME = "Name";
	static constexpr std::string_view FIELD_FEATURE_TYPE = "FeatureType";
	static constexpr std::string_view FIELD_ISLAND_ID = "IslandId";

	void Feature::Initialize()
	{
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int Feature::Create(int islandId, const std::string& name, int featureType, int district)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, islandId, common::Data::QuoteString(name), featureType, district);
		return Common::LastInsertedIndex();
	}

	std::vector<int> Feature::ReadForIslandDistrict(int islandId, int district)
	{
		Initialize();
		auto records = Common::Execute(QUERY_FOR_ISLAND_DISTRICT, islandId, district);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_FEATURE_ID));
		return result;
	}

	std::optional<std::string> Feature::ReadName(int featureId)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, featureId),
			FIELD_NAME);
	}

	std::optional<int> Feature::ReadFeatureType(int featureId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_FEATURE_TYPE, featureId),
			FIELD_FEATURE_TYPE);
	}

	std::optional<int> Feature::ReadIslandId(int featureId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_ISLAND_ID, featureId),
			FIELD_ISLAND_ID);
	}
}