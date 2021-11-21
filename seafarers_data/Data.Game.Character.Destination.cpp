#include <Common.Data.h>
#include "Data.Game.Character.Destination.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Known.h"
#include "Data.Game.Player.h"
#include <string>
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterDestinations]([CharacterId] INT NOT NULL,[DestinationId] INT NOT NULL, [X] REAL NOT NULL, [Y] REAL NOT NULL, UNIQUE([CharacterId],[DestinationId]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [CharacterDestinations] WHERE [CharacterId]={} AND [DestinationId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterDestinations]([CharacterId],[DestinationId],[X],[Y]) VALUES({},{},{:.4f},{:.4f});";
	static const std::string DELETE_ITEM = "DELETE FROM [CharacterDestinations] WHERE [CharacterId]={} AND [DestinationId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [CharacterDestinations] WHERE [CharacterId]={};";

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void Destination::Write(int characterId, int destinationId, const common::XY<double>& location)
	{
		AutoCreateTable();
		Common::Execute(
			REPLACE_ITEM,
			characterId,
			destinationId,
			location.GetX(),
			location.GetY());
	}

	std::optional<common::XY<double>> Destination::Read(int characterId, int destinationId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, destinationId);
		if (!records.empty())
		{
			return Common::ToXY(records.front());
		}
		return std::nullopt;
	}

	void Destination::Clear(int characterId, int destinationId)
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ITEM, 
			characterId, 
			destinationId);
	}

	void Destination::ClearAll(int characterId)
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ALL, 
			characterId);
	}
}