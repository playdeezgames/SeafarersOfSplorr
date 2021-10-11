#include <Common.RNG.h>
#include "Game.Items.h"
#include <map>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::map<game::Commodity, double> commodities;
		size_t present;
		size_t notPresent;
		double tonnage;
		std::map<Difficulty, size_t> initialInventories;
		std::optional<size_t> avatarRationWeight;
	};

	static const std::map<game::Item, game::ItemDescriptor> itemTable =
	{
		{game::Item::RATIONS,
			{
				"hardtack",
				{
					{game::Commodity::WHEAT, 0.1}
				},
				1,
				0,
				0.02,
				{},
				1
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
				0.1,
				{},
				std::nullopt
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
				0.25,
				{},
				std::nullopt
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
				0.02,
				{},
				std::nullopt
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
				0.0,
				{},
				std::nullopt
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
				0.1,
				{
					{Difficulty::EASY, 1},
					{Difficulty::NORMAL, 1},
					{Difficulty::HARD, 1}
				},
				std::nullopt
			}
		},
		{game::Item::FISHING_POLE,
			{
				"fishing pole",
				{
					{game::Commodity::WOOD, 0.25},
					{game::Commodity::SILK, 0.25}
				},
				1,
				1,
				0.3,
				{
					//{Difficulty::EASY, 1}
				},
				std::nullopt
			}
		},
		{game::Item::BAIT,
			{
				"bait",
				{
					{game::Commodity::MEAT, 0.01}
				},
				1,
				1,
				0.001,
				{
					//{Difficulty::EASY, 10}
				},
				std::nullopt
			}
		},
		{ game::Item::SEAWEED,
			{
				"seaweed",
				{
					{game::Commodity::SEAWEED, 1.0}
				},
				1,
				1,
				0.00001,
				{
				},
				std::nullopt
			}
		},
		{ game::Item::POSH_TROUSERS,
			{
				"posh trousers",
				{
					{game::Commodity::SILK, 1.0},
					{game::Commodity::LABOR, 0.75}
				},
				1,
				9,
				0.1,
				{
				},
				std::nullopt
			}
		},
		{ game::Item::SALTED_FISH,
			{
				"salted fish",
				{
					{game::Commodity::MEAT, 0.2},
					{game::Commodity::SALT, 0.2},
					{game::Commodity::LABOR, 0.1}
				},
				1,
				1,
				0.02,
				{
				},
				0
			}
		}
	};

	static std::list<game::Item> itemList;

	static const game::ItemDescriptor& Read(const game::Item& item)
	{
		return itemTable.find(item)->second;
	}

	const std::list<game::Item>& Items::All()
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

	static std::map<game::Item, size_t> rationsGenerator;

	Item Items::GenerateRations()
	{
		if (rationsGenerator.empty())
		{
			for (auto& entry : itemTable)
			{
				if (entry.second.avatarRationWeight.has_value())
				{
					rationsGenerator[entry.first] = entry.second.avatarRationWeight.value();
				}
			}
		}
		return common::RNG::FromGenerator(rationsGenerator, rationsGenerator.begin()->first);
	}

	const std::string& Items::GetName(const game::Item& item)
	{
		return Read(item).name;
	}

	bool Items::GeneratePresence(const game::Item& item)
	{
		auto& descriptor = Read(item);
		return common::RNG::FromRange(0u, descriptor.present + descriptor.notPresent) < descriptor.present;
	}

	const std::map<Commodity, double>& Items::GetCommodities(const game::Item& item)
	{
		return Read(item).commodities;
	}

	double Items::GetUnitTonnage(const game::Item& item)
	{
		return Read(item).tonnage;
	}

	const std::map<Difficulty, size_t>& Items::GetInitialInventories(const game::Item& item)
	{
		return Read(item).initialInventories;
	}

}