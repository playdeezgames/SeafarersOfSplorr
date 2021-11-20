#include <Data.Game.Avatar.Rations.h>
#include "Game.Avatar.Rations.h"
#include <Game.Items.h>
namespace game::avatar
{
	std::optional<game::Item> Rations::Read(int avatarId)
	{
		auto itemId = data::game::avatar::Rations::Read(avatarId);
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Rations::Write(int avatarId, const std::optional<game::Item>& item)
	{
		data::game::avatar::Rations::Clear(avatarId);
		if (item)
		{
			data::game::avatar::Rations::Write(avatarId, (int)item.value());
		}
	}
}