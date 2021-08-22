#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.h"
#include <format>
namespace data::game::island::DarkAlley
{
	const std::string FIELD_INFAMY_REQUIREMENT = "InfamyRequirement";
	const std::string FIELD_RUFFIAN_BRAWLING_STRENGTH = "RuffianBrawlingStrength";
	const std::string FIELD_MINIMUM_WAGER = "MinimumWager";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DarkAlleys]([X] REAL NOT NULL,[Y] REAL NOT NULL,[InfamyRequirement] REAL NOT NULL,[RuffianBrawlingStrength] REAL NOT NULL, [MinimumWager] REAL NOT NULL, UNIQUE([X],[Y]));";
	const std::string DELETE_ALL = "DELETE FROM [DarkAlleys];";
	const std::string REPLACE_ITEM = "REPLACE INTO [DarkAlleys]([X],[Y],[InfamyRequirement],[RuffianBrawlingStrength],[MinimumWager]) VALUES({:.4f},{:.4f},{},{},{});";
	const std::string QUERY_ITEM = "SELECT [InfamyRequirement],[RuffianBrawlingStrength],[MinimumWager] FROM [DarkAlleys] WHERE [X]={:.4f} AND [Y]={:.4f};";

	const auto AutoCreateDarkAlleysTable = data::game::Common::Run(CREATE_TABLE);

	void Clear()
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void Write(const common::XY<double>& location, const DarkAlleyData& data)
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(
			std::format(
				REPLACE_ITEM, 
				location.GetX(), 
				location.GetY(), 
				data.infamyRequirement, 
				data.ruffianBrawlingStrength,
				data.minimumWager));
	}

	std::optional<DarkAlleyData> Read(const common::XY<double>& location)
	{
		AutoCreateDarkAlleysTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, location.GetX(), location.GetY()));
		if (!records.empty())
		{
			auto record = records.front();
			return std::optional<DarkAlleyData>({
				common::Data::ToDouble(record[FIELD_INFAMY_REQUIREMENT]),
				common::Data::ToDouble(record[FIELD_RUFFIAN_BRAWLING_STRENGTH]),
				common::Data::ToDouble(record[FIELD_MINIMUM_WAGER])
				});
		}
		return std::nullopt;
	}
}