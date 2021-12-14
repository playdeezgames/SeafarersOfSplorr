#include "Game.Characters.Items.h"
#include "Game.Session.Character.LegacyItems.h"
namespace game::session::character
{
	LegacyItems::LegacyItems(int characterId)
		: characterId(characterId)
	{

	}

	bool LegacyItems::Has(const game::Item& item) const
	{
		return game::characters::Items::Has(characterId, item);
	}

	double LegacyItems::GetTonnage() const
	{
		return game::characters::Items::TotalTonnage(characterId);
	}

	std::list<character::LegacyItem> LegacyItems::GetAll() const
	{
		auto items = game::characters::Items::All(characterId);
		std::list<character::LegacyItem> result;
		for (auto item : items)
		{
			result.push_back(character::LegacyItem(characterId, item.first));
		}
		return result;
	}
}