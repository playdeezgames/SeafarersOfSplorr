#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.DemigodFavor.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterDemigodFavor]([CharacterId] INT NOT NULL,[DemigodName] TEXT NOT NULL,[Favor] REAL NOT NULL, UNIQUE([CharacterId],[DemigodName]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterDemigodFavor]([CharacterId],[DemigodName],[Favor]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [Favor] FROM [CharacterDemigodFavor] WHERE [CharacterId]={} AND [DemigodName]={};";
	static const std::string DELETE_ALL = "DELETE FROM [CharacterDemigodFavor] WHERE [CharacterId]={};";

	static const std::string FIELD_FAVOR = "Favor";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void DemigodFavor::Write(int characterId, const std::string& name, double favor)
	{
		AutoCreateTable();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			common::Data::QuoteString(name), 
			favor);
	}

	std::optional<double> DemigodFavor::Read(int characterId, const std::string& name)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, common::Data::QuoteString(name));
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
