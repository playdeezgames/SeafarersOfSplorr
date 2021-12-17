#include <Data.Game.Character.Counter.h>
#include "Game.Session.Character.Counter.h"
namespace game::session::character
{
	Counter::Counter(int characterId, const game::characters::Counter& counter)
		: characterId(characterId)
		, counter(counter)
	{

	}

	int Counter::Change(int delta) const
	{
		auto counterValue = data::game::character::Counter::Read(characterId, (int)counter).value();
		auto newValue = counterValue + delta;
		data::game::character::Counter::Write(characterId, (int)counter, newValue);
		return newValue;
	}
}