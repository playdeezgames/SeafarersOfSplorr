#pragma once
#include "Cards.Card.h"
#include <string>
#include "Common.Card.h"
namespace visuals::CardSprites
{
	const std::string& GetSpriteForCard(const cards::Card&);
}