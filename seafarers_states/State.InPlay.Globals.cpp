#include <Common.Utility.Optional.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Quest.h>
#include <Game.Character.Statistics.h>
#include <Game.Character.Ship.h>
#include <Game.Islands.h>
#include <Game.Player.h>
#include <Game.Ship.h>
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

	std::optional<game::Quest> GetPlayerCharacterQuest()
	{
		return game::character::Quest::Read(GetPlayerCharacterId());
	}

	std::optional<common::XY<double>> GetPlayerCharacterShipLocation()
	{
		return
			common::utility::Optional::Bind<int, common::XY<double>>(
				GetPlayerCharacterShipId(),
				game::Ship::GetLocation);
	}

	std::optional<std::list<game::Island>> GetPlayerCharacterViewableIslands()
	{
		return common::utility::Optional::Map<int, std::list<game::Island>>(
			GetPlayerCharacterShipId(),
			game::Islands::GetViewableIslands);
	}

	std::optional<double> GetPlayerCharacterShipHeading()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterShipId(),
			game::Ship::GetHeading);
	}

	std::optional<double> GetPlayerCharacterShipSpeed()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterShipId(),
			game::Ship::GetSpeed);
	}

	void PlayMainTheme()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	std::list<game::Island> GetPlayerCharacterKnownIslands()
	{
		return game::Islands::GetKnownIslands(GetPlayerCharacterId());
	}

	void SetPlayerCharacterShipHeading(double heading)
	{
		auto shipId = GetPlayerCharacterShipId();
		if (shipId)
		{
			game::Ship::SetHeading(shipId.value(), heading);
		}
	}

	void DoPlayerCharacterAction(const game::character::Action& action)
	{
		game::character::Actions::DoAction(GetPlayerCharacterId(), action);
	}

	void ChangePlayerCharacterMoney(double delta)
	{
		game::character::Statistics::ChangeMoney(GetPlayerCharacterId(), delta);
	}
}
