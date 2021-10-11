#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.DestinationName.h"
#include "Data.Game.Player.h"
namespace data::game::avatar//20211011
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DestinationNames]([AvatarId] INT NOT NULL,[DestinationId] INT NOT NULL, [DestinationName] TEXT NOT NULL, UNIQUE([AvatarId],[DestinationId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DestinationNames]([AvatarId],[DestinationId],[DestinationName]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [DestinationName] FROM [DestinationNames] WHERE [AvatarId]={} AND [DestinationId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [DestinationNames] WHERE [AvatarId]={};";

	static const std::string FIELD_DESTINATION_NAME = "DestinationName";

	static const auto AutoCreateDestinationNamesTable = Common::Run(CREATE_TABLE);

	std::optional<std::string> DestinationName::Read(int avatarId, int index)
	{
		AutoCreateDestinationNamesTable();
		auto records = Common::Execute(QUERY_ITEM, avatarId, index);
		if (!records.empty())
		{
			return records.front()[FIELD_DESTINATION_NAME];
		}
		return std::nullopt;
	}

	void DestinationName::Write(int avatarId, int index, const std::string& name)
	{
		AutoCreateDestinationNamesTable();
		Common::Execute(
				REPLACE_ITEM,
				avatarId,
				index,
				common::Data::QuoteString(name));
	}

	void DestinationName::Clear(int avatarId)
	{
		AutoCreateDestinationNamesTable();
		Common::Execute(
			DELETE_ALL, 
			avatarId);
	}
}