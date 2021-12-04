#include <Data.Game.Character.Rations.h>
#include "Game.Characters.Rations.h"
#include "Game.Items.h"
namespace game::character
{
	std::optional<game::Item> Rations::Read(int characterId)
	{
		auto itemId = data::game::character::Rations::Read(characterId);
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Rations::Write(int characterId, const std::optional<game::Item>& item)
	{
		data::game::character::Rations::Clear(characterId);
		if (item)
		{
			data::game::character::Rations::Write(characterId, (int)item.value());
		}
	}
}