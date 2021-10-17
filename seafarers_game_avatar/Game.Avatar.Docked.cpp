#include <Data.Game.Avatar.h>
#include <Data.Game.Avatar.Dock.h>
#include <Data.Game.Island.DarkAlley.h>
#include <format>
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.StateTransition.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Colors.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include <Game.Player.h>
#include <map>
namespace game::avatar//20211011
{
	void Docked::Reset(const game::Difficulty&)
	{

	}

	static const std::string FORMAT_DOCK = "You dock at {}!";

	static void SetAvatarStateToDocked()
	{
		auto avatar = data::game::Avatar::Read(Player::GetAvatarId()).value();
		avatar.state = (int)game::avatar::State::DOCK;
		data::game::Avatar::Write(Player::GetAvatarId(), avatar);
	}

	static std::optional<DockResult> DoDock(const common::XY<double>& location)
	{
		std::optional<DockResult> result = DockResult::DOCKED;
		game::Islands::AddVisit(
			location,
			Statistics::GetTurnsRemaining());
		game::islands::Quests::Update(location);
		if (game::avatar::Quest::CompleteQuest(location))
		{
			result = DockResult::COMPLETED_QUEST;
		}
		data::game::avatar::Dock::Write(Player::GetAvatarId(), location);
		SetAvatarStateToDocked();
		auto island = game::Islands::Read(location).value();
		Log::Write({ game::Colors::GREEN, std::format(FORMAT_DOCK, island.name) });
		return result;
	}

	std::optional<DockResult> Docked::Dock()
	{
		if (ReadLocation().has_value())
		{
			return DockResult::ALREADY_DOCKED;
		}
		auto dockables = game::Islands::GetDockableIslands();
		if (!dockables.empty())
		{
			return DoDock(dockables.front().absoluteLocation);
		}
		return std::nullopt;
	}

	std::optional<common::XY<double>> Docked::ReadLocation()
	{
		return data::game::avatar::Dock::Read(Player::GetAvatarId());
	}
}