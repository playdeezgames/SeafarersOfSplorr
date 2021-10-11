#include <Common.Data.h>
#include "Data.Game.Avatar.Destination.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Known.h"
#include "Data.Game.Player.h"
#include <string>
namespace data::game::avatar//20211011
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarDestinations]([AvatarId] INT NOT NULL,[DestinationId] INT NOT NULL, [X] REAL NOT NULL, [Y] REAL NOT NULL, UNIQUE([AvatarId],[DestinationId]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [AvatarDestinations] WHERE [AvatarId]={} AND [DestinationId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarDestinations]([AvatarId],[DestinationId],[X],[Y]) VALUES({},{},{:.4f},{:.4f});";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarDestinations] WHERE [AvatarId]={} AND [DestinationId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarDestinations] WHERE [AvatarId]={};";

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";

	static const auto AutoCreateAvatarDestinationsTable = data::game::Common::Run(CREATE_TABLE);

	void Destination::Write(int avatarId, int destinationId, const common::XY<double>& location)
	{
		AutoCreateAvatarDestinationsTable();
		Common::Execute(
			REPLACE_ITEM,
			avatarId,
			destinationId,
			location.GetX(),
			location.GetY());
	}

	std::optional<common::XY<double>> Destination::Read(int avatarId, int destinationId)
	{
		AutoCreateAvatarDestinationsTable();
		auto records = Common::Execute(QUERY_ITEM, avatarId, destinationId);
		if (!records.empty())
		{
			return Common::ToXY(records.front());
		}
		return std::nullopt;
	}

	void Destination::Clear(int avatarId, int destinationId)
	{
		AutoCreateAvatarDestinationsTable();
		Common::Execute(
			DELETE_ITEM, 
			avatarId, 
			destinationId);
	}

	void Destination::ClearAll(int avatarId)
	{
		AutoCreateAvatarDestinationsTable();
		Common::Execute(
			DELETE_ALL, 
			avatarId);
	}
}