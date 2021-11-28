#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Feature.h"
namespace data::game::island
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandFeatures]([IslandId] INT NOT NULL,[FeatureId] INT NOT NULL,UNIQUE([IslandId],[FeatureId]));";
	static const std::string DELETE_ALL = "DELETE FROM [IslandFeatures];";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandFeatures]([IslandId],[FeatureId]) VALUES({},{});";
	static const std::string QUERY_ITEM = "SELECT [FeatureId] FROM [IslandFeatures] WHERE [IslandId]={} AND [FeatureId]={};";
	static const std::string DELETE_ITEM = "DELETE FROM [IslandFeatures] WHERE [IslandId]={} AND [FeatureId]={};";

	static const auto AutoCreateIslandFeaturesTable = data::game::Common::Run(CREATE_TABLE);

	void Feature::Clear()
	{
		AutoCreateIslandFeaturesTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void Feature::Write(int islandId, int featureId)
	{
		AutoCreateIslandFeaturesTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			islandId, 
			featureId);
	}

	void Feature::Clear(int islandId, int featureId)
	{
		AutoCreateIslandFeaturesTable();
		data::game::Common::Execute(
			DELETE_ITEM, 
			islandId, 
			featureId);
	}

	bool Feature::Read(int islandId, int featureId)
	{
		AutoCreateIslandFeaturesTable();
		return !data::game::Common::Execute(
			QUERY_ITEM, 
			islandId, 
			featureId).empty();
	}
}
