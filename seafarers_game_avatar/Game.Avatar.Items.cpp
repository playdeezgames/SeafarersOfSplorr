#include <Data.Game.Avatar.Items.h>
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
namespace game::avatar
{
	std::map<game::Item, size_t> Items::All()
	{
		std::map<game::Item, size_t> result;
		auto inventory = data::game::avatar::Items::All();
		for (auto& item : inventory)
		{
			result[(game::Item)item.first] = item.second;
		}
		return result;
	}

	size_t Items::Read(const game::Item& item)
	{
		return data::game::avatar::Items::Read((int)item);
	}

	bool Items::Has(const Item& item)
	{
		return Read(item) > 0;
	}


	void Items::Add(const game::Item& item, const size_t& count)
	{
		auto previous = Read(item);
		data::game::avatar::Items::Write((int)item, previous + count);
	}

	void Items::Remove(const game::Item& item, const size_t& count)
	{
		auto previous = Read(item);
		data::game::avatar::Items::Write((int)item, (previous>=count) ? (previous - count) : (0));
	}

	double Items::TotalTonnage()
	{
		double tonnage = 0.0;
		for (auto& entry : All())
		{
			auto unitTonnage = game::Items::GetUnitTonnage(entry.first);
			tonnage += unitTonnage * (double)entry.second;
		}
		return tonnage;
	}

	void Items::Reset(const game::Difficulty& difficulty)
	{
		data::game::avatar::Items::Clear();
		for (auto& item : game::Items::All())
		{
			auto& initialInventories = game::Items::GetInitialInventories(item);
			auto iter = initialInventories.find(difficulty);
			if (iter != initialInventories.end())
			{
				Add(item, iter->second);
			}
		}
	}
}