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
					{game::Commodity::WHEAT, 0.01}
				},
				1,
				0
			}
		},
		{game::Item::SACK_OF_WHEAT,
			{
				"sack of wheat",
				{
					{game::Commodity::WHEAT, 1.0}
				},
				1,
				1
			}
		}
	};

	const game::ItemDescriptor& Read(const game::Item& item)
	{
		return itemTable.find(item)->second;
	}
}