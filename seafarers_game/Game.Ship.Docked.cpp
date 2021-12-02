#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.Docks.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Island.h>
#include "Game.Character.h"
#include "Game.Character.State.h"
#include "Game.Character.Quest.h"
#include "Game.Character.Ship.h"
#include "Game.Character.Statistics.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include "Game.Ship.Docked.h"
#include <set>
namespace game::ship
{
	static std::optional<DockResult> DoDock(int characterId, const common::XY<double>& location)
	{
		std::optional<DockResult> result = DockResult::DOCKED;
		game::Islands::AddVisit(
			data::game::Island::Find(location).value(),
			game::character::statistics::Turns::Remaining(characterId).value());
		game::islands::Quests::Update(characterId, data::game::Island::Find(location).value());
		int islandId = data::game::Island::Find(location).value();
		if (game::character::Quest::Complete(characterId, islandId))
		{
			result = DockResult::COMPLETED_QUEST;
		}
		int shipId = data::game::character::Ship::ReadForCharacter(characterId).value().shipId;
		data::game::ship::Docks::Write(shipId, islandId);
		game::Character::DoAction(characterId, game::character::Action::ENTER_DOCK);
		auto island = game::Islands::Read(islandId).value();
		return result;
	}

	std::optional<DockResult> Docked::Dock(int shipId)
	{
		if (GetIsland(shipId))
		{
			return DockResult::ALREADY_DOCKED;
		}
		auto dockables = game::Islands::GetDockableIslands(shipId);
		if (!dockables.empty())
		{
			auto billets = data::game::character::Ship::ReadForShip(shipId);
			std::set<DockResult> dockResults;
			for (auto billet : billets)
			{
				auto dockResult = DoDock(billet.characterId, dockables.front().absoluteLocation);
				if (dockResult)
				{
					dockResults.insert(dockResult.value());
				}
			}
			if (dockResults.contains(DockResult::COMPLETED_QUEST))
			{
				return DockResult::COMPLETED_QUEST;
			}
			return DockResult::DOCKED;
		}
		return std::nullopt;
	}

	std::optional<int> Docked::GetIsland(int shipId)
	{
		return data::game::ship::Docks::Read(shipId);
	}

	std::optional<bool> Docked::Undock(int shipId)
	{
		if (data::game::Ship::Read(shipId))
		{
			if (!GetIsland(shipId))
			{
				return false;
			}
			auto billets = data::game::character::Ship::ReadForShip(shipId);
			//first time, check that all billets are at the dock
			for (auto billet : billets)
			{
				auto characterState = game::Character::GetState(billet.characterId).value();
				if (characterState != game::character::State::DOCK)
				{
					return false;
				}
			}
			//second time, put them on the boat
			for (auto billet : billets)
			{
				game::Character::DoAction(billet.characterId, game::character::Action::UNDOCK);
			}
			data::game::ship::Docks::Clear(shipId);
			return true;
		}
		return std::nullopt;
	}

}