#include <Data.Game.Island.Feature.h>
#include "Game.Session.Island.Feature.h"
namespace game::session::island
{
	std::string Feature::GetName() const
	{
		return data::game::island::Feature::ReadName(featureId).value();
	}
}