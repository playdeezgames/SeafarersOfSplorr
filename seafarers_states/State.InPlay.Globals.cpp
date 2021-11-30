#include <Data.Game.Character.Ship.h>
#include <Game.Player.h>
#include <Game.Ship.Docked.h>
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	int GetPlayerCharacterId()
	{
		return game::Player::GetCharacterId();
	}

	static int GetPlayerCharacterShipId()
	{
		return data::game::character::Ship::ReadForCharacter(GetPlayerCharacterId()).value().shipId;
	}

	std::optional<int> GetPlayerCharacterIslandId()
	{
		return game::ship::Docked::GetIsland(GetPlayerCharacterShipId());
	}

	std::optional<game::ship::DockResult> Dock()
	{
		return game::ship::Docked::Dock(GetPlayerCharacterShipId());
	}
}
