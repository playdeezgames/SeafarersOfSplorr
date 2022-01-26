#include "Data.Game.Item.Type.Property.h"
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
namespace data::game::item::type
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE_INT =
		R"(CREATE TABLE IF NOT EXISTS [ItemTypeIntProperties]
		(
			[ItemTypeId] INT NOT NULL,
			[Key] TEXT NOT NULL,
			[Value] INT NOT NULL,
			UNIQUE([ItemTypeId],[Key]),
			FOREIGN KEY([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"sv;
	static constexpr auto REPLACE_INT =
		R"(REPLACE INTO [ItemTypeIntProperties]
		(
			[ItemTypeId],
			[Key],
			[Value]
		) 
		VALUES({},{},{});)"sv;
	static constexpr auto QUERY_INT =
		R"(SELECT 
			[Value] 
		FROM [ItemTypeIntProperties] 
		WHERE 
			[ItemTypeId]={} AND 
			[Key]={};)"sv;
	static constexpr auto COLUMN_VALUE = "Value"sv;

	void Property::InitializeInt()
	{
		data::game::item::Type::Initialize();
		Common::Execute(CREATE_TABLE_INT);
	}

	void Property::WriteInt(int itemTypeId, const std::string& key, int value)
	{
		InitializeInt();
		Common::Execute(REPLACE_INT, itemTypeId, common::Data::QuoteString(key), value);
	}

	std::optional<int> Property::ReadInt(int itemTypeId, const std::string& key)
	{
		InitializeInt();
		return Common::TryToInt(
			Common::TryExecuteForOne(
				QUERY_INT, 
				itemTypeId, 
				common::Data::QuoteString(key)),
			COLUMN_VALUE);
	}
}