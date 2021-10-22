#include <Common.Utility.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Avatar.Items.h>
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
#include <Game.Player.h>
namespace game::avatar//20211017
{
	std::map<game::Item, size_t> Items::All()
	{
		return
			common::utility::Table::MapTable<int, size_t, Item, size_t>(
				data::game::avatar::Items::All(Player::GetAvatarId()),
				common::Utility::Cast<int, Item>,
				common::Utility::Identity<size_t>);
	}

	size_t Items::Read(const game::Item& item)
	{
		return data::game::avatar::Items::Read(Player::GetAvatarId(), (int)item);
	}

	bool Items::Has(const Item& item)
	{
		return Read(item) > 0;
	}


	void Items::Add(const game::Item& item, const size_t& count)
	{
		data::game::avatar::Items::Write(Player::GetAvatarId(), (int)item, Read(item) + count);
	}

	void Items::Remove(const game::Item& item, const size_t& count)
	{
		auto previous = Read(item);
		data::game::avatar::Items::Write(Player::GetAvatarId(), (int)item, (previous>=count) ? (previous - count) : (0));
	}

	double Items::TotalTonnage()
	{
		return common::utility::Table::AccumulateTable<Item, size_t, double>(
			All(),
			[](const double& result, const Item& item, const size_t count) 
			{ 
				return result + game::Items::GetUnitTonnage(item) * (double)count;
			});
	}

	void Items::Reset(const game::Difficulty& difficulty)
	{
		data::game::avatar::Items::Clear(Player::GetAvatarId());
		for (auto& item : game::Items::All())
		{
			common::Utility::IterateOptional<size_t>(
				common::utility::Table::TryGetKey(game::Items::GetInitialInventoriesForAvatar(item), difficulty),
				[item](const size_t& count) { Add(item, count); }
			);
		}
	}
}