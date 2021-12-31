#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.DemigodFavor.h"
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterDemigodFavor]
		(
			[CharacterId] INT NOT NULL,
			[DemigodId] INT NOT NULL,
			[Favor] REAL NOT NULL, 
			UNIQUE([CharacterId],[DemigodId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([DemigodId]) REFERENCES [Demigods]([DemigodId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterDemigodFavor]
		(
			[CharacterId],
			[DemigodId],
			[Favor]
		) 
		VALUES({},{},{},{});)"s;
	static const std::string QUERY_ITEM_FAVOR = 
		R"(SELECT 
			[Favor] 
		FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"s;
	static const std::string DELETE_ALL_FOR_CHARACTER = 
		R"(DELETE FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [CharacterDemigodFavor];)"s;
	static const std::string UPDATE_ITEM_COLUMN =
		R"(UPDATE [CharacterDemigodFavor] 
		SET 
			[{}]={} 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"s;

	static const std::string FIELD_CHARACTER_ID = "CharacterId";
	static const std::string FIELD_DEMIGOD_ID = "DemigodId";
	static const std::string FIELD_FAVOR = "Favor";

	void DemigodFavor::Initialize()
	{
		Character::Initialize();
		Demigod::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void DemigodFavor::Write(int characterId, int demigodId, double favor)
	{
		Initialize();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			demigodId, 
			favor);
	}

	std::optional<double> DemigodFavor::Read(int characterId, int demigodId)
	{
		Initialize();
		return Common::TryToDouble(
			Common::TryExecuteForOne(QUERY_ITEM_FAVOR, characterId, demigodId),
			FIELD_FAVOR);
	}

	void DemigodFavor::ClearAll()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}
