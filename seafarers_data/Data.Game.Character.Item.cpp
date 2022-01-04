#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.h"
#include "Data.Game.Item.h"
#include "Data.Game.Character.Item.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterItems]
		(
			[ItemId] INT NOT NULL UNIQUE,
			[CharacterId] INT NOT NULL,
			FOREIGN KEY ([ItemId]) REFERENCES [Items]([ItemId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterItems]
		(
			[ItemId],
			[CharacterId]
		) 
		VALUES({},{});)"s;
	static const std::string QUERY_ITEMS_FOR_CHARACTER = 
		R"(SELECT 
			[ItemId] 
		FROM [CharacterItems] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string QUERY_CHARACTER_FOR_ITEM = 
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterItems] 
		WHERE 
			[ItemId]={};)"s;

	static const std::string FIELD_CHARACTER_ID = "CharacterId";
	static const std::string FIELD_ITEM_ID = "ItemId";

	void Item::Initialize()
	{
		Character::Initialize();
		data::game::Item::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Item::Write(int itemInstanceId, int characterId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, itemInstanceId, characterId);
	}

	std::vector<int> Item::ReadForCharacter(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEMS_FOR_CHARACTER, characterId);
		std::vector<int> result(records.size());
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ITEM_ID));
		return result;
	}

	std::optional<int> Item::ReadForItemInstance(int itemInstanceId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_CHARACTER_FOR_ITEM, itemInstanceId),
			FIELD_CHARACTER_ID);
	}
}