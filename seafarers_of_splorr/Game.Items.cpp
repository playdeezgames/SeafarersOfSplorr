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
				}
			}
		}
	};

	const game::ItemDescriptor& Read(const game::Item& item)
	{
		return itemTable.find(item)->second;
	}
}