#include <algorithm>
#include "Data.Game.Character.h"
#include "Data.Game.Character.DemigodFavor.h"
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterDemigodFavor]
		(
			[CharacterId] INT NOT NULL,
			[DemigodId] INT NOT NULL,
			[Favor] REAL NOT NULL, 
			UNIQUE([CharacterId],[DemigodId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([DemigodId]) REFERENCES [Demigods]([DemigodId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterDemigodFavor]
		(
			[CharacterId],
			[DemigodId],
			[Favor]
		) 
		VALUES({},{},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM_FAVOR = 
		R"(SELECT 
			[Favor] 
		FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"sv;
	static constexpr std::string_view DELETE_ALL_FOR_CHARACTER = 
		R"(DELETE FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view DELETE_ALL = 
		R"(DELETE FROM [CharacterDemigodFavor];)"sv;
	static constexpr std::string_view UPDATE_ITEM_COLUMN =
		R"(UPDATE [CharacterDemigodFavor] 
		SET 
			[{}]={} 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"sv;

	static constexpr std::string_view FIELD_CHARACTER_ID = "CharacterId";
	static constexpr std::string_view FIELD_DEMIGOD_ID = "DemigodId";
	static constexpr std::string_view FIELD_FAVOR = "Favor";

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
