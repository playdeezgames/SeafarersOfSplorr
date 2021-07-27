#include "Game.Items.h"
#include <map>
namespace game::Items
{
	std::map<game::Item, game::ItemDescriptor> itemTable =
	{
		{game::Item::RATIONS,
			{
				"rations",
				{
					{game::Commodity::WHEAT, 0.1}
				},
				1,
				0,
				0.001
			}
		},
		{game::Item::SACK_OF_WHEAT,
			{
				"sack of wheat",
				{
					{game::Commodity::WHEAT, 1.0}
				},
				1,
				0,
				0.1
			}
		},
		{game::Item::CLOTH,
			{
				"cloth",
				{
					{game::Commodity::WOOL, 0.25},
					{game::Commodity::LABOR, 0.1}
				},
				3,
				1,
				0.25
			}
		},
		{game::Item::FISH,
			{
				"fish",
				{
					{game::Commodity::MEAT, 0.2}
				},
				5,
				1,
				0.02
			}
		},
		{game::Item::JOOLS,
			{
				"jools",
				{
					{game::Commodity::JOOLS, 1.0}
				},
				1,
				0,
				0.0
			}
		},
		{game::Item::TROUSERS,
			{
				"trousers",
				{
					{game::Commodity::WOOL, 1.0},
					{game::Commodity::LABOR, 0.75}
				},
				1,
				1,
				0.05
			}
		}
	};

	static std::list<game::Item> itemList;

	const game::ItemDescriptor& Read(const game::Item& item)
	{
		return itemTable.find(item)->second;
	}

	const std::list<game::Item>& All()
	{
		if (itemList.empty())
		{
			for (auto entry : itemTable)
			{
				itemList.push_back(entry.first);
			}
		}
		return itemList;
	}

}