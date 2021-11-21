#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.Plight.h"
#include "Data.Game.Player.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Plights]([AvatarId] INT NOT NULL,[PlightId] INT NOT NULL, [Duration] INT NULL, UNIQUE([AvatarId],[PlightId]));";
	static const std::string DELETE_ALL = "DELETE FROM [Plights] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL_PLIGHTS = "DELETE FROM [Plights];";
	static const std::string DELETE_ITEM = "DELETE FROM [Plights] WHERE [PlightId]={} AND [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Plights]([AvatarId], [PlightId], [Duration]) VALUES({}, {}, {});";
	static const std::string QUERY_ITEM = "SELECT [PlightId], [Duration] FROM [Plights] WHERE [PlightId]={} AND [AvatarId]={};";
	static const std::string QUERY_ALL = "SELECT [PlightId], [Duration] FROM [Plights] WHERE [AvatarId]={};";

	static const std::string FIELD_PLIGHT_ID = "PlightId";
	static const std::string FIELD_DURATION = "Duration";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	static Plight ToPlight(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_PLIGHT_ID)->second),
			common::Data::ToOptionalInt(record.find(FIELD_DURATION)->second)
		};
	}

	std::optional<Plight> Plight::Read(int characterId, int plightId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, plightId, characterId);
		if (!records.empty())
		{
			return ToPlight(records.front());
		}
		return std::nullopt;
	}

	void Plight::Write(int characterId, const Plight& plight)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, characterId, plight.plightId, common::Data::OfOptional(plight.duration));
	}

	void Plight::ClearPlight(int characterId, int plightId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ITEM, plightId, characterId);

	}
	void Plight::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL,characterId);
	}

	std::list<Plight> Plight::All(int characterId)
	{
		AutoCreateTable();
		std::list<Plight> result;
		auto records = Common::Execute(QUERY_ALL, characterId);
		for (auto& record : records)
		{
			result.push_back(ToPlight(record));
		}
		return result;
	}

	void Plight::ClearAll()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL_PLIGHTS);
	}
}