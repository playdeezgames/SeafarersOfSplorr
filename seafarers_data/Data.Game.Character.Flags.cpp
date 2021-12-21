#include <Common.Data.h>
#include "Data.Game.Character.Flags.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	const static std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [CharacterFlags]"
		"("
			"[CharacterId] INT NOT NULL,"
			"[FlagId] INT NOT NULL,"
			"UNIQUE([CharacterId],[FlagId])"
		");";
	const static std::string QUERY_ITEM = 
		"SELECT "
			"[FlagId] "
		"FROM [CharacterFlags] "
		"WHERE "
			"[CharacterId]={} "
			"AND [FlagId]={};";
	const static std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterFlags]"
		"("
			"[CharacterId],"
			"[FlagId]"
		") "
		"VALUES({},{});";
	const static std::string DELETE_ITEM = 
		"DELETE FROM [CharacterFlags] "
		"WHERE "
			"[CharacterId]={} "
			"AND [FlagId]={};";
	const static std::string QUERY_ALL_FOR_AVATAR = 
		"SELECT "
			"[FlagId] "
		"FROM [CharacterFlags] "
		"WHERE "
			"[CharacterId]={};";

	const static std::string FIELD_FLAG_ID = "FlagId";

	static auto AutoCreateTable = Common::Run(CREATE_TABLE);

	bool Flags::Has(int characterId, int flagId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, flagId);
		return !records.empty();
	}

	void Flags::Write(int characterId, int flagId)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, characterId, flagId);
	}

	void Flags::Clear(int characterId, int flagId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ITEM, characterId, flagId);
	}

	std::set<int> Flags::All(int characterId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ALL_FOR_AVATAR, characterId);
		std::set<int> result;
		for (auto& record : records)
		{
			result.insert(common::Data::ToInt(record[FIELD_FLAG_ID]));
		}
		return result;
	}
}