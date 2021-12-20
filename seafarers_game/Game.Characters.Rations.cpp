#include <Data.Game.Character.RationsLegacy.h>
#include "Game.Characters.Rations.h"
#include "Game.Items.h"
namespace game::characters
{
	std::optional<game::Item> Rations::Read(int characterId)
	{
		auto itemId = data::game::character::RationsLegacy::Read(characterId);
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Rations::Write(int characterId, const std::optional<game::Item>& item)
	{
		data::game::character::RationsLegacy::Clear(characterId);
		if (item)
		{
			data::game::character::RationsLegacy::Write(characterId, (int)item.value());
		}
	}
}