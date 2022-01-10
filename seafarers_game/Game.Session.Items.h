#pragma once
#include "Game.Item.Category.h"
#include "Game.Session.Item.h"
namespace game::session
{
	struct Items
	{
		constexpr Items() {}
		Item Add(const item::Type&, int) const;
		Item GetItem(int) const;
		void Purge() const;
		void Reset() const;
		void ApplyTurnEffects() const;
	};
}