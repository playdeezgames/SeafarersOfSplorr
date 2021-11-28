#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.DemigodFavor.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterDemigodFavor]([CharacterId] INT NOT NULL,[DemigodId] INT NOT NULL,[Favor] REAL NOT NULL, UNIQUE([CharacterId],[DemigodId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterDemigodFavor]([CharacterId],[DemigodId],[Favor]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [Favor] FROM [CharacterDemigodFavor] WHERE [CharacterId]={} AND [DemigodId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [CharacterDemigodFavor] WHERE [CharacterId]={};";

	static const std::string FIELD_FAVOR = "Favor";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void DemigodFavor::Write(int characterId, int demigodId, double favor)
	{
		AutoCreateTable();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			demigodId, 
			favor);
	}

	std::optional<double> DemigodFavor::Read(int characterId, int demigodId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, demigodId);
		if (!records.empty())
		{
			return common::Data::ToOptionalDouble(records.front()[FIELD_FAVOR]);
		}
		return std::nullopt;
	}

	void DemigodFavor::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL, characterId);
	}
}
