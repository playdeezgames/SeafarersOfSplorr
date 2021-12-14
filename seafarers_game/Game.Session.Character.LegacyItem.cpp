#include "Game.Characters.Items.h"
#include "Game.Items.h"
#include "Game.Session.Character.LegacyItem.h"
namespace game::session::character
{
	LegacyItem::LegacyItem(int characterId, const game::Item& item)
		: characterId(characterId)
		, item(item)
	{

	}

	std::string LegacyItem::GetName() const
	{
		return game::Items::GetName(item);
	}

	size_t LegacyItem::GetCount() const
	{
		return game::characters::Items::Read(characterId, item);
	}
}