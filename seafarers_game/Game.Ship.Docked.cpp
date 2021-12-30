#include <Data.Game.Character.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.CurrentIsland.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Island.h>
#include "Game.Characters.State.h"
#include "Game.Characters.Ships.h"
#include "Game.Session.h"
#include "Game.Ship.Docked.h"
#include <set>
namespace game::ship
{
	static std::optional<DockResult> DoDock(int characterId, int islandId)
	{
		std::optional<DockResult> result = DockResult::DOCKED;
		data::game::character::KnownIsland::Write(
			characterId, 
			islandId);
		int shipId = data::game::character::Ship::ReadShipForCharacter(characterId).value();
		data::game::ship::CurrentIsland::Write(shipId, islandId);
		game::Session().GetCharacters().GetCharacter(characterId).DoAction(game::characters::Action::ENTER_DOCK);
		return result;
	}

	std::optional<DockResult> Docked::Dock(int shipId)
	{
		if (GetIsland(shipId))
		{
			return DockResult::ALREADY_DOCKED;
		}
		auto dockable = game::Session().GetShips().GetShip(shipId).GetDockableIslands().TryGetFirst();
		if (dockable)
		{
			auto billets = data::game::character::Ship::ReadCharactersForShip(shipId);
			std::set<DockResult> dockResults;
			for (auto billet : billets)
			{
				auto dockResult = DoDock(billet, dockable.value().operator int());
				if (dockResult)
				{
					dockResults.insert(dockResult.value());
				}
			}
			return DockResult::DOCKED;
		}
		return std::nullopt;
	}

	std::optional<int> Docked::GetIsland(int shipId)
	{
		return data::game::ship::CurrentIsland::Read(shipId);
	}

	std::optional<bool> Docked::Undock(int shipId)
	{
		if (data::game::Ship::GetHeading(shipId).has_value())
		{
			if (!GetIsland(shipId))
			{
				return false;
			}
			auto billets = data::game::character::Ship::ReadCharactersForShip(shipId);
			//first time, check that all billets are at the dock
			for (auto billet : billets)
			{
				auto characterState = game::Session().GetCharacters().GetCharacter(billet).GetState();
				if (characterState != game::characters::State::DOCK)
				{
					return false;
				}
			}
			//second time, put them on the boat
			for (auto billet : billets)
			{
				game::Session().GetCharacters().GetCharacter(billet).DoAction(game::characters::Action::UNDOCK);
			}
			data::game::ship::CurrentIsland::Clear(shipId);
			return true;
		}
		return std::nullopt;
	}

}