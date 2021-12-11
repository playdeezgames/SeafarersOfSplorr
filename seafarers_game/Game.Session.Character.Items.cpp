#include "Game.Characters.Items.h"
#include "Game.Session.Character.Items.h"
namespace game::session::character
{
	Items::Items(int characterId)
		: characterId(characterId)
	{

	}

	bool Items::Has(const game::Item& item) const
	{
		return game::characters::Items::Has(characterId, item);
	}

}