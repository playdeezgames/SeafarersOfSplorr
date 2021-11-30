#include <Game.Character.Docked.h>
#include <Game.Player.h>
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	int GetPlayerCharacterId()
	{
		return game::Player::GetCharacterId();
	}

	std::optional<int> GetPlayerCharacterIslandId()
	{
		return game::character::Docked::GetIsland(GetPlayerCharacterId());
	}
}
