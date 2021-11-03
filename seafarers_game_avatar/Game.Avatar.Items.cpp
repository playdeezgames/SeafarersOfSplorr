#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Avatar.Items.h>
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
#include <Game.Player.h>
namespace game::avatar
{
	std::map<game::Item, size_t> Items::All(int avatarId)
	{
		return
			common::utility::Table::Map<int, size_t, Item, size_t>(
				data::game::avatar::Items::All(avatarId),
				common::Utility::Cast<int, Item>,
				common::Utility::Identity<size_t>);
	}

	size_t Items::Read(int avatarId, const game::Item& item)
	{
		return data::game::avatar::Items::Read(avatarId, (int)item);
	}

	bool Items::Has(int avatarId, const Item& item)
	{
		return Read(avatarId, item) > 0;
	}


	void Items::Add(int avatarId, const game::Item& item, const size_t& count)
	{
		data::game::avatar::Items::Write(avatarId, (int)item, Read(avatarId, item) + count);
	}

	void Items::Remove(int avatarId, const game::Item& item, const size_t& count)
	{
		auto previous = Read(avatarId, item);
		data::game::avatar::Items::Write(avatarId, (int)item, (previous>=count) ? (previous - count) : (0));
	}

	double Items::TotalTonnage(int avatarId)
	{
		return common::utility::Table::Accumulate<Item, size_t, double>(
			All(avatarId),
			[](double& result, const Item& item, const size_t count) 
			{ 
				double delta = game::Items::GetUnitTonnage(item) * (double)count;
				result += delta;
			});
	}

	void Items::Reset(const game::Difficulty& difficulty)
	{
		data::game::avatar::Items::Clear(Player::GetAvatarId());
		for (auto& item : game::Items::All())
		{
			common::utility::Optional::Iterate<size_t>(
				common::utility::Table::TryGetKey(game::Items::GetInitialInventoriesForAvatar(item), difficulty),
				[item](const size_t& count) { Add(Player::GetAvatarId(), item, count); }
			);
		}
	}
}