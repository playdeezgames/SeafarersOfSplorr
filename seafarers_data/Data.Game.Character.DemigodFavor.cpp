#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.DemigodFavor.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterDemigodFavor]([CharacterId] INT NOT NULL,[DemigodId] INT NOT NULL,[Favor] REAL NOT NULL, [OfferingCooldown] INT NOT NULL, UNIQUE([CharacterId],[DemigodId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterDemigodFavor]([CharacterId],[DemigodId],[Favor],[OfferingCooldown]) VALUES({},{},{},{});";
	static const std::string QUERY_ITEM_FAVOR = "SELECT [Favor] FROM [CharacterDemigodFavor] WHERE [CharacterId]={} AND [DemigodId]={};";
	static const std::string QUERY_ITEM_OFFERING_COOLDOWN = "SELECT [OfferingCooldown] FROM [CharacterDemigodFavor] WHERE [CharacterId]={} AND [DemigodId]={};";
	static const std::string DELETE_ALL_FOR_CHARACTER = "DELETE FROM [CharacterDemigodFavor] WHERE [CharacterId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [CharacterDemigodFavor];";

	static const std::string FIELD_FAVOR = "Favor";
	static const std::string FIELD_OFFERING_COOLDOWN = "OfferingCooldown";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void DemigodFavor::Write(int characterId, int demigodId, double favor, int offeringCooldown)
	{
		AutoCreateTable();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			demigodId, 
			favor,
			offeringCooldown);
	}

	std::optional<double> DemigodFavor::ReadFavor(int characterId, int demigodId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM_FAVOR, characterId, demigodId);
		if (!records.empty())
		{
			return common::Data::ToOptionalDouble(records.front()[FIELD_FAVOR]);
		}
		return std::nullopt;
	}

	std::optional<int> DemigodFavor::ReadOfferingCooldown(int characterId, int demigodId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM_OFFERING_COOLDOWN, characterId, demigodId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_OFFERING_COOLDOWN]);
		}
		return std::nullopt;
	}

	void DemigodFavor::ClearAll()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL);
	}
}
