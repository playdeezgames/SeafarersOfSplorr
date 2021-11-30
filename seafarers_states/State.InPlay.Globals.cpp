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
		return game::character::Docked::GetIsland(GetPlayerCharacterId());
	}

	std::optional<game::character::DockResult> Dock()
	{
		return game::character::Docked::Dock(GetPlayerCharacterId());
	}

}
