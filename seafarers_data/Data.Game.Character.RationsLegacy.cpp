#include <Common.Data.h>
#include "Data.Game.Character.RationsLegacy.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [CharacterRations]"
		"("
			"[CharacterId] INT NOT NULL UNIQUE,"
			"[ItemId] INT NOT NULL"
		");";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterRations]"
		"("
			"[CharacterId],"
			"[ItemId]"
		") "
		"VALUES({},{});";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[ItemId] "
		"FROM [CharacterRations] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string DELETE_ITEM = 
		"DELETE FROM [CharacterRations] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string DELETE_ALL = 
		"DELETE FROM [CharacterRations];";

	static const std::string FIELD_ITEM_ID = "ItemId";

	static auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void RationsLegacy::Write(int characterId, int itemId)
	{
		AutoCreateTable();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			itemId);
	}

	std::optional<int> RationsLegacy::Read(int characterId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_ID]);
		}
		return std::nullopt;
	}

	void RationsLegacy::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ITEM, 
			characterId);
	}

	void RationsLegacy::ClearAll()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL);
	}
}