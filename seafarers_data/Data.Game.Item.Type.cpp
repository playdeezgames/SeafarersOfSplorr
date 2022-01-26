#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
namespace data::game::item
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ItemTypes]
		(
			[ItemTypeId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[Name] TEXT NOT NULL
		);)"sv;
	static constexpr std::string_view INSERT_ITEM =
		R"(INSERT INTO [ItemTypes]
		(
			[Name]
		) 
		VALUES({});)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [ItemTypes] 
		WHERE 
			[ItemTypeId]={};)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [ItemTypes];)"sv;

	static constexpr std::string_view FIELD_MAXIMUM_TYPE = "MaximumType";
	static constexpr std::string_view FIELD_NAME = "Name";

	void Type::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	int Type::Create(const std::string& name)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, common::Data::QuoteString(name));
		return Common::LastInsertedIndex();
	}

	std::optional<std::string> Type::ReadName(int itemType)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, itemType),
			FIELD_NAME);
	}
	
	void Type::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}
