#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.DestinationName.h"
#include "Data.Game.Player.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterDestinationNames]([CharacterId] INT NOT NULL,[DestinationId] INT NOT NULL, [DestinationName] TEXT NOT NULL, UNIQUE([CharacterId],[DestinationId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterDestinationNames]([CharacterId],[DestinationId],[DestinationName]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [DestinationName] FROM [CharacterDestinationNames] WHERE [CharacterId]={} AND [DestinationId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [CharacterDestinationNames] WHERE [CharacterId]={};";

	static const std::string FIELD_DESTINATION_NAME = "DestinationName";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	std::optional<std::string> DestinationName::Read(int characterId, int index)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, index);
		if (!records.empty())
		{
			return records.front()[FIELD_DESTINATION_NAME];
		}
		return std::nullopt;
	}

	void DestinationName::Write(int characterId, int index, const std::string& name)
	{
		AutoCreateTable();
		Common::Execute(
				REPLACE_ITEM,
				characterId,
				index,
				common::Data::QuoteString(name));
	}

	void DestinationName::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ALL, 
			characterId);
	}
}