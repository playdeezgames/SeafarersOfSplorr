#include "Game.Characters.Items.h"
#include "Game.Session.Character.LegacyItems.h"
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

	double Items::GetTonnage() const
	{
		return game::characters::Items::TotalTonnage(characterId);
	}

	std::list<character::Item> Items::GetAll() const
	{
		auto items = game::characters::Items::All(characterId);
		std::list<character::Item> result;
		for (auto item : items)
		{
			result.push_back(character::Item(characterId, item.first));
		}
		return result;
	}
}