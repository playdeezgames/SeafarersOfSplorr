#include "Data.Game.Item.Type.Category.h"
#include "Data.Game.Item.Type.h"
#include <Common.Data.h>
#include "Data.Game.Common.h"
namespace data::game::item::type
{
	using namespace std::string_view_literals;

	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [ItemTypeCategories]
		(
			[ItemTypeId] INT NOT NULL,
			[Category] INT NOT NULL,
			UNIQUE([ItemTypeId],[Category]),
			FOREIGN KEY ([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [ItemTypeCategories]
		(
			[ItemTypeId],
			[Category]
		) 
		VALUES({},{});)"sv;

	void Category::Initialize()
	{
		data::game::item::Type::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Category::AddToCategory(int itemTypeId, int category)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, itemTypeId, category);
	}
}