#include "Data.Game.Common.h"
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
			[District] INT NOT NULL
		);)"s;

	void Feature::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}
}