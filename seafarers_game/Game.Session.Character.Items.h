#pragma once
#include "Game.Item.h"
#include "Game.Session.Character.Item.h"
#include <list>
namespace game::session::character
{
	struct Items
	{
		Items(int);
		bool Has(const game::Item&) const;
		double GetTonnage() const;
		std::list<character::Item> GetAll() const;
	private:
		int characterId;
	};
}