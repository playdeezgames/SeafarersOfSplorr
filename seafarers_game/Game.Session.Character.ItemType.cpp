#include <Data.Game.Character.ItemType.h>
#include "Game.Session.Character.ItemType.h"
namespace game::session::character
{
	int ItemType::GetQuantity() const
	{
		return data::game::character::ItemType::Read(characterId, itemTypeId).value_or(0);
	}
}