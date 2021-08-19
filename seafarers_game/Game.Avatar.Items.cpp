#include "Data.Game.Avatar.Items.h"
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
namespace game::avatar::Items
{
	std::map<game::Item, size_t> All()
	{
		std::map<game::Item, size_t> result;
		auto inventory = data::game::avatar::Items::ReadAll();
		for (auto& item : inventory)
		{
			result[(game::Item)item.first] = item.second;
		}
		return result;
	}

	size_t Read(const game::Item& item)
	{
		return data::game::avatar::Items::Read((int)item);
	}

	void Add(const game::Item& item, const size_t& count)
	{
		auto previous = Read(item);
		data::game::avatar::Items::Write((int)item, previous + count);
	}

	void Remove(const game::Item& item, const size_t& count)
	{
		auto previous = Read(item);
		data::game::avatar::Items::Write((int)item, (previous>=count) ? (previous - count) : (0));
	}

	double TotalTonnage()
	{
		double tonnage = 0.0;
		for (auto& entry : All())
		{
			auto& descriptor = game::Items::Read(entry.first);
			tonnage += descriptor.tonnage * (double)entry.second;
		}
		return tonnage;
	}

	void Reset(const game::Difficulty&)
	{
		data::game::avatar::Items::Clear();
	}
}