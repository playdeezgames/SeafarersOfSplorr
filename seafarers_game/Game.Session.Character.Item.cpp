#include "Game.Characters.Items.h"
#include "Game.Items.h"
#include "Game.Session.Character.Item.h"
namespace game::session::character
{
	Item::Item(int characterId, const game::Item& item)
		: characterId(characterId)
		, item(item)
	{

	}

	std::string Item::GetName() const
	{
		return game::Items::GetName(item);
	}

	size_t Item::GetCount() const
	{
		return game::characters::Items::Read(characterId, item);
	}
}