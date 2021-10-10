#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.h"
namespace data::game::island//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DarkAlleys]([X] REAL NOT NULL,[Y] REAL NOT NULL,[InfamyRequirement] REAL NOT NULL,[RuffianBrawlingStrength] REAL NOT NULL, [MinimumWager] REAL NOT NULL, UNIQUE([X],[Y]));";
	static const std::string DELETE_ALL = "DELETE FROM [DarkAlleys];";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DarkAlleys]([X],[Y],[InfamyRequirement],[RuffianBrawlingStrength],[MinimumWager]) VALUES({:.4f},{:.4f},{},{},{});";
	static const std::string QUERY_ITEM = "SELECT [InfamyRequirement],[RuffianBrawlingStrength],[MinimumWager] FROM [DarkAlleys] WHERE [X]={:.4f} AND [Y]={:.4f};";

	static const std::string FIELD_INFAMY_REQUIREMENT = "InfamyRequirement";
	static const std::string FIELD_RUFFIAN_BRAWLING_STRENGTH = "RuffianBrawlingStrength";
	static const std::string FIELD_MINIMUM_WAGER = "MinimumWager";

	static const auto AutoCreateDarkAlleysTable = data::game::Common::Run(CREATE_TABLE);

	void DarkAlley::Clear()
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void DarkAlley::Write(const common::XY<double>& location, const DarkAlley& data)
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(
				REPLACE_ITEM, 
				location.GetX(), 
				location.GetY(), 
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

	std::optional<DarkAlley> DarkAlley::Read(const common::XY<double>& location)
	{
		AutoCreateDarkAlleysTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, location.GetX(), location.GetY());
		if (!records.empty())
		{
			return ToDarkAlley(records.front());
		}
		return std::nullopt;
	}
}