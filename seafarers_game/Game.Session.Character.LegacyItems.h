#pragma once
#include "Game.ItemLegacy.h"
#include "Game.Session.Character.LegacyItem.h"
#include <list>
namespace game::session::character
{
	struct LegacyItems
	{
		LegacyItems(int);
		bool Has(const game::Item&) const;
		double GetTonnage() const;
		std::list<character::LegacyItem> GetAll() const;
	private:
		int characterId;
	};
}