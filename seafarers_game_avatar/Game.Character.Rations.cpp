#include <Data.Game.Character.Rations.h>
#include "Game.Character.Rations.h"
#include <Game.Items.h>
namespace game::character
{
	std::optional<game::Item> Rations::Read(int avatarId)
	{
		auto itemId = data::game::character::Rations::Read(avatarId);
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Rations::Write(int avatarId, const std::optional<game::Item>& item)
	{
		data::game::character::Rations::Clear(avatarId);
		if (item)
		{
			data::game::character::Rations::Write(avatarId, (int)item.value());
		}
	}
}