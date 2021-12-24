#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
namespace data::game::item
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ItemTypes]
		(
			[ItemTypeId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[Category] INT NOT NULL,
			[Type] INT NOT NULL,
			[Name] TEXT NOT NULL,
			UNIQUE([Category],[Type])
		);)"s;
	static const std::string INSERT_ITEM = 
		R"(INSERT INTO [ItemTypes]
		(
			[Category],
			[Type],
			[Name]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_MAXIMUM_TYPE = 
		R"(SELECT 
			MAX([Type]) AS MaximumType 
		FROM [ItemTypes] 
		WHERE 
			[Category]={};)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [ItemTypes] 
		WHERE 
			[ItemTypeId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [ItemTypes];)"s;

	static const std::string FIELD_MAXIMUM_TYPE = "MaximumType";
	static const std::string FIELD_TYPE = "Type";
	static const std::string FIELD_CATEGORY = "Category";
	static const std::string FIELD_NAME = "Name";

	void Type::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	int Type::ReadNextTypeForCategory(int category)
	{
		Initialize();
		auto records = Common::Execute(QUERY_MAXIMUM_TYPE, category);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_MAXIMUM_TYPE]) + 1;
		}
		return 0;
	}

	int Type::EstablishTypeForCategory(int category, int type, const std::string& name)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, category, type, common::Data::QuoteString(name));
		return Common::LastInsertedIndex();
	}

	std::optional<int> Type::ReadCategory(int itemType)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_COLUMN, FIELD_CATEGORY, itemType);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_CATEGORY]);
		}
		return std::nullopt;
	}

	std::optional<int> Type::ReadType(int itemType)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_COLUMN, FIELD_TYPE, itemType);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_TYPE]);
		}
		return std::nullopt;
	}

	std::optional<std::string> Type::ReadName(int itemType)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_COLUMN, FIELD_NAME, itemType);
		if (!records.empty())
		{
			return records.front()[FIELD_NAME];
		}
		return std::nullopt;
	}
	
	void Type::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}
