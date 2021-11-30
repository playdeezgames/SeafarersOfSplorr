#include <Game.Player.h>
#include <Game.Ship.Docked.h>
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	int GetPlayerCharacterId()
	{
		return game::Player::GetCharacterId();
	}

	std::optional<int> GetPlayerCharacterIslandId()
	{
		return game::ship::Docked::GetIsland(GetPlayerCharacterId());
	}

	std::optional<game::ship::DockResult> Dock()
	{
		return game::ship::Docked::Dock(GetPlayerCharacterId());
	}

}
