#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Feature.h"
#include <string>
namespace data::game::island
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Features]
		(
			[FeatureId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[IslandId] INT NOT NULL,
			[Name] TEXT NOT NULL,
			[FeatureType] INT NOT NULL,
			[District] INT NOT NULL,
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"s;
	static const std::string INSERT_ITEM =
		R"(INSERT INTO [Features]
		(
			[IslandId],
			[Name],
			[FeatureType],
			[District]
		) 
		VALUES({},{},{},{});)"s;

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
}