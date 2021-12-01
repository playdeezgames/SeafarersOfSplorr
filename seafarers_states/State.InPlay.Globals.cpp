#include <Common.Utility.Optional.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Items.h>
#include <Game.Character.Quest.h>
#include <Game.Character.Statistics.h>
#include <Game.Character.Ship.h>
#include <Game.Character.ShipStatistics.h>
#include <Game.Islands.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.Items.h>
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
		common::utility::Optional::Iterate<int>(
			GetPlayerCharacterShipId(),
			[heading](int shipId) {game::Ship::SetHeading(shipId, heading); }
			);
	}

	void DoPlayerCharacterAction(const game::character::Action& action)
	{
		game::character::Actions::DoAction(GetPlayerCharacterId(), action);
	}

	void ChangePlayerCharacterMoney(double delta)
	{
		game::character::Statistics::ChangeMoney(GetPlayerCharacterId(), delta);
	}

	std::optional<common::XY<double>> GetPlayerCharacterIslandLocation()
	{
		auto islandId = GetPlayerCharacterIslandId();
		if (islandId)
		{
			auto island = game::Islands::Read(islandId.value());
			if (island)
			{
				return island.value().absoluteLocation;
			}
		}
		return std::nullopt;
	}

	double GetPlayerCharacterMoney()
	{
		return game::character::Statistics::ReadMoney(GetPlayerCharacterId());
	}

	std::optional<double> GetPlayerCharacterDarkAlleyAnte()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterIslandId(),
			game::islands::DarkAlley::GetAnte);
	}

	std::optional<double> GetPlayerCharacterDarkAlleyMinimumWager()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterIslandId(),
			game::islands::DarkAlley::GetMinimumWager);
	}

	std::optional<double> GetPlayerCharacterAvailableTonnage()
	{
		return game::character::Ship::AvailableTonnage(GetPlayerCharacterId());
	}

	std::optional<std::map<game::Item, double>> GetPlayerCharacterPurchasePrices()
	{
		return common::utility::Optional::Bind<int, std::map<game::Item, double>>(
			GetPlayerCharacterIslandId(),
			game::islands::Items::GetPurchasePrices);
	}
	std::map<game::Item, size_t> GetPlayerCharacterItems()
	{
		return game::character::Items::All(GetPlayerCharacterId());
	}

	void PlayerCharacterCleanHull(const game::Side& side)
	{
		game::character::ShipStatistics::CleanHull(GetPlayerCharacterShipId().value(), side);
	}

	static game::Item islandTradeItem = game::Item::RATIONS;

	void SetIslandTradeItem(const game::Item& item)
	{
		islandTradeItem = item;
	}

	const game::Item& GetIslandTradeItem()
	{
		return islandTradeItem;
	}

}
