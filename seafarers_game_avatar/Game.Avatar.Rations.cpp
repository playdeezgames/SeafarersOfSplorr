#include <Data.Game.Avatar.Rations.h>
#include "Game.Avatar.Rations.h"
#include <Game.Items.h>
#include <Game.Player.h>
namespace game::avatar//20211018
{
	std::optional<game::Item> Rations::Read()
	{
		auto itemId = data::game::avatar::Rations::Read(Player::GetAvatarId());
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Rations::Write(const std::optional<game::Item>& item)
	{
		data::game::avatar::Rations::Clear(Player::GetAvatarId());
		if (item)
		{
			data::game::avatar::Rations::Write(Player::GetAvatarId(), (int)item.value());
		}
	}
}