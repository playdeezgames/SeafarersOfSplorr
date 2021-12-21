#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.h"
namespace data::game::island
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [DarkAlleys]"
		"("
			"[IslandId] INT NOT NULL,"
			"[InfamyRequirement] REAL NOT NULL,"
			"[RuffianBrawlingStrength] REAL NOT NULL, "
			"[MinimumWager] REAL NOT NULL, "
			"UNIQUE([IslandId])"
		");";
	static const std::string DELETE_ALL = 
		"DELETE FROM [DarkAlleys];";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [DarkAlleys]"
		"("
			"[IslandId],"
			"[InfamyRequirement],"
			"[RuffianBrawlingStrength],"
			"[MinimumWager]"
		") "
		"VALUES({},{},{},{});";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[InfamyRequirement],"
			"[RuffianBrawlingStrength],"
			"[MinimumWager] "
		"FROM [DarkAlleys] "
		"WHERE "
			"[IslandId]={};";

	static const std::string FIELD_INFAMY_REQUIREMENT = "InfamyRequirement";
	static const std::string FIELD_RUFFIAN_BRAWLING_STRENGTH = "RuffianBrawlingStrength";
	static const std::string FIELD_MINIMUM_WAGER = "MinimumWager";

	static const auto AutoCreateDarkAlleysTable = data::game::Common::Run(CREATE_TABLE);

	void DarkAlley::Clear()
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void DarkAlley::Write(int islandId, const DarkAlley& data)
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(
				REPLACE_ITEM, 
				islandId, 
				data.infamyRequirement, 
				data.ruffianBrawlingStrength,
				data.minimumWager);
	}

	static DarkAlley ToDarkAlley(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToDouble(record.find(FIELD_INFAMY_REQUIREMENT)->second),
			common::Data::ToDouble(record.find(FIELD_RUFFIAN_BRAWLING_STRENGTH)->second),
			common::Data::ToDouble(record.find(FIELD_MINIMUM_WAGER)->second)
		};
	}

	std::optional<DarkAlley> DarkAlley::Read(int islandId)
	{
		AutoCreateDarkAlleysTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, islandId);
		if (!records.empty())
		{
			return ToDarkAlley(records.front());
		}
		return std::nullopt;
	}
}