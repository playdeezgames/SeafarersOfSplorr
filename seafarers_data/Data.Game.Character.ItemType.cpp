#include <algorithm>
#include "Data.Game.Character.h"
#include "Data.Game.Item.Type.h"
#include "Data.Game.Character.ItemType.h"
#include "Data.Game.Common.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterItemTypes]
		(
			[CharacterId] INT NOT NULL,
			[ItemTypeId] INT NOT NULL,
			[Quantity] INT NOT NULL,
			UNIQUE([CharacterId],[ItemTypeId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [CharacterItemTypes]
		(
			[CharacterId],
			[ItemTypeId],
			[Quantity]
		) 
		VALUES({},{},{});)"sv;
	static constexpr auto QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [CharacterItemTypes] 
		WHERE 
			[CharacterId]={}
			AND [ItemTypeId]={};)"sv;
	static constexpr auto DELETE_ITEM =
		R"(DELETE FROM [CharacterItemTypes] 
		WHERE 
			[CharacterId]={} 
			AND [ItemTypeId]={};)"sv;
	static constexpr auto QUERY_ITEM_TYPES_FOR_CHARACTER =
		R"(SELECT 
			[ItemTypeId] 
		FROM [CharacterItemTypes] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto QUERY_ITEM_TYPE_COUNT_FOR_CHARACTER =
		R"(SELECT 
			COUNT([ItemTypeId]) AS [ItemTypeCount] 
		FROM [CharacterItemTypes] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto FIELD_QUANTITY = "Quantity"sv;
	static constexpr auto FIELD_ITEM_TYPE_ID = "ItemTypeId"sv;
	static constexpr auto FIELD_ITEM_TYPE_COUNT = "ItemTypeCount"sv;

	void ItemType::Initialize()
	{
		Character::Initialize();
		data::game::item::Type::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void ItemType::Write(int characterId, int itemTypeId, int quantity)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, itemTypeId, quantity);
	}

	void ItemType::Clear(int characterId, int itemTypeId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, itemTypeId);
	}

	std::optional<int> ItemType::Read(int characterId, int itemTypeId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_QUANTITY, characterId, itemTypeId),
			FIELD_QUANTITY);
	}

	std::vector<int> ItemType::ReadForCharacter(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_TYPES_FOR_CHARACTER, characterId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ITEM_TYPE_ID));
		return result;
	}

	std::optional<int> ItemType::ReadCountForCharacter(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_TYPE_COUNT_FOR_CHARACTER, characterId), 
			FIELD_ITEM_TYPE_COUNT);
	}

}