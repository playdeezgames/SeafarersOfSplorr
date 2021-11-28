#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.Docks.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.h>
#include <format>
#include <functional>
#include "Game.Character.h"
#include "Game.Character.Docked.h"
#include "Game.Character.StateTransition.h"
#include "Game.Character.Quest.h"
#include "Game.Character.Ship.h"
#include "Game.Character.Statistics.h"
#include "Game.Colors.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include <map>
namespace game::character
{
	void Docked::Reset(const game::Difficulty&)
	{

	}

	static const std::string FORMAT_DOCK = "You dock at {}!";

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
			game::character::Statistics::GetTurnsRemaining(characterId));
		game::islands::Quests::Update(location);
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

	std::optional<DockResult> Docked::Dock(int characterId)
	{
		if (ReadLocation(characterId).has_value())
		{
			return DockResult::ALREADY_DOCKED;
		}
		auto dockables = game::Islands::GetDockableIslands(game::character::Ship::ReadShipId(characterId).value());
		if (!dockables.empty())
		{
			return DoDock(characterId, dockables.front().absoluteLocation);
		}
		return std::nullopt;
	}

	std::optional<int> Docked::ReadLocation(int characterId)
	{
		auto ship = data::game::character::Ship::ReadForCharacter(characterId);
		if (ship)
		{
			return data::game::ship::Docks::Read(ship.value().shipId);
		}
		return std::nullopt;
	}
}