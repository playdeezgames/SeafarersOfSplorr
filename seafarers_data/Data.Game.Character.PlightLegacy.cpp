#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.PlightLegacy.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Plights]([CharacterId] INT NOT NULL,[PlightId] INT NOT NULL, [Duration] INT NULL, UNIQUE([CharacterId],[PlightId]));";
	static const std::string DELETE_ALL = "DELETE FROM [Plights] WHERE [CharacterId]={};";
	static const std::string DELETE_ALL_PLIGHTS = "DELETE FROM [Plights];";
	static const std::string DELETE_ITEM = "DELETE FROM [Plights] WHERE [PlightId]={} AND [CharacterId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Plights]([CharacterId], [PlightId], [Duration]) VALUES({}, {}, {});";
	static const std::string QUERY_ITEM = "SELECT [PlightId], [Duration] FROM [Plights] WHERE [PlightId]={} AND [CharacterId]={};";
	static const std::string QUERY_ALL = "SELECT [PlightId], [Duration] FROM [Plights] WHERE [CharacterId]={};";

	static const std::string FIELD_PLIGHT_ID = "PlightId";
	static const std::string FIELD_DURATION = "Duration";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	static PlightLegacy ToPlight(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_PLIGHT_ID)->second),
			common::Data::ToOptionalInt(record.find(FIELD_DURATION)->second)
		};
	}

	std::optional<PlightLegacy> PlightLegacy::Read(int characterId, int plightId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, plightId, characterId);
		if (!records.empty())
		{
			return ToPlight(records.front());
		}
		return std::nullopt;
	}

	void PlightLegacy::Write(int characterId, const PlightLegacy& plight)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, characterId, plight.plightId, common::Data::OfOptional(plight.duration));
	}

	void PlightLegacy::ClearPlight(int characterId, int plightId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ITEM, plightId, characterId);

	}
	void PlightLegacy::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL,characterId);
	}

	std::list<PlightLegacy> PlightLegacy::All(int characterId)
	{
		AutoCreateTable();
		std::list<PlightLegacy> result;
		auto records = Common::Execute(QUERY_ALL, characterId);
		for (auto& record : records)
		{
			result.push_back(ToPlight(record));
		}
		return result;
	}

	void PlightLegacy::ClearAll()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL_PLIGHTS);
	}
}