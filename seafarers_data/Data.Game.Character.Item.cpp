#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.h"
#include "Data.Game.Item.h"
#include "Data.Game.Character.ItemInstance.h"
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

	void ItemInstance::Initialize()
	{
		Character::Initialize();
		Item::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void ItemInstance::Write(int itemInstanceId, int characterId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, itemInstanceId, characterId);
	}

	std::list<int> ItemInstance::ReadForCharacter(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEMS_FOR_CHARACTER, characterId);
		std::list<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ITEM_ID));
		return result;
	}

	std::optional<int> ItemInstance::ReadForItemInstance(int itemInstanceId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_CHARACTER_FOR_ITEM, itemInstanceId),
			FIELD_CHARACTER_ID);
	}
}