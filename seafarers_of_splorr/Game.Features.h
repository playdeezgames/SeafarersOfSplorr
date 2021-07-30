#pragma once
#include "Game.Feature.h"
#include "Game.FeatureDescriptor.h"
#include <list>
namespace game::Features
{
	const game::FeatureDescriptor& Read(const game::Feature&);
	const std::list<game::Feature>& All();
}
