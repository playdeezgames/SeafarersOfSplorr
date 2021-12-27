#include <Data.Game.Character.h>
#include "Game.Demigods.h"
#include "Game.Session.Demigods.h"
namespace game::session
{
	void Demigods::Reset() const
	{
		game::Demigods::Reset();
	}

	void Demigods::Populate(const Difficulty& difficulty) const
	{
		game::Demigods::Populate(difficulty);
	}

	void Demigods::ApplyTurnEffects() const
	{
		auto characterIds = data::game::Character::All();
		for (auto characterId : characterIds)
		{
			game::Demigods::ApplyTurnEffects(characterId);
		}
	}
}