#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.Docks.h>
#include <Data.Game.Island.h>
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
	static void SetAvatarStateToDocked(int characterId)
	{
		auto avatar = data::game::Character::Read(characterId).value();
		avatar.state = (int)game::character::State::DOCK;
		data::game::Character::Write(characterId, avatar);
	}

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
		SetAvatarStateToDocked(characterId);
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


}