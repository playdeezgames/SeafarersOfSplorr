#include <Common.Utility.Optional.h>
#include <Game.Character.Ship.h>
#include <Game.Player.h>
#include <Game.Ship.Docked.h>
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	int GetPlayerCharacterId()
	{
		return game::Player::GetCharacterId();
	}

	std::optional<int> GetPlayerCharacterShipId()
	{
		return game::character::Ship::ReadShipId(GetPlayerCharacterId());
	}

	std::optional<int> GetPlayerCharacterIslandId()
	{
		return
			common::utility::Optional::Bind<int, int>(
				GetPlayerCharacterShipId(),
				game::ship::Docked::GetIsland);
	}

	std::optional<game::ship::DockResult> Dock()
	{
		return 
			common::utility::Optional::Bind<int, game::ship::DockResult>(
				GetPlayerCharacterShipId(), 
				game::ship::Docked::Dock);
	}
}
