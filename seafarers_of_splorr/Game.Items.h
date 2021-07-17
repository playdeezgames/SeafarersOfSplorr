#pragma once
#include "Game.Item.h"
#include "Game.ItemDescriptor.h"
#include <list>
namespace game::Items
{
	const game::ItemDescriptor& Read(const game::Item&);
	const std::list<game::Item>& All();
}
