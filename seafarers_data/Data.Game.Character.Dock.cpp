#include <Common.Data.h>
#include "Data.Game.Character.Dock.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <string>
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterDocks]([CharacterId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [X], [Y] FROM [CharacterDocks] WHERE [CharacterId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterDocks]([CharacterId],[X],[Y]) VALUES({},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [CharacterDocks] WHERE [CharacterId] = {};";

	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";

	static const auto AutoCreateAvatarDockTable = data::game::Common::Run(CREATE_TABLE);

	void Dock::Clear(int characterId)
	{
		AutoCreateAvatarDockTable();
		data::game::Common::Execute(
			DELETE_ITEM, 
			characterId);
	}


	void Dock::Write(int characterId, const common::XY<double>& location)
	{
		AutoCreateAvatarDockTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			location.GetX(), 
			location.GetY());
	}

	std::optional<common::XY<double>> Dock::Read(int characterId)
	{
		AutoCreateAvatarDockTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return Common::ToXY(records.front());
		}
		return std::nullopt;
	}
}