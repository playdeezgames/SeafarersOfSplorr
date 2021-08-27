#include <Data.Game.Avatar.Plight.h>
#include "Game.Avatar.Plights.h"
namespace game::avatar
{
	bool Plights::Has(const Plight& plightId)
	{
		return data::game::avatar::Plight::Read((int)plightId).has_value();
	}

	void Plights::ApplyTurnEffects()
	{
		//TODO: tick down the counters for the plights
	}
}