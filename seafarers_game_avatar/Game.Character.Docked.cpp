#include <Data.Game.Character.h>
#include <Data.Game.Character.Dock.h>
#include <Data.Game.Island.DarkAlley.h>
#include <format>
#include <functional>
#include "Game.Character.h"
#include "Game.Character.Docked.h"
#include "Game.Character.StateTransition.h"
#include "Game.Character.Quest.h"
#include "Game.Character.Statistics.h"
#include "Game.Colors.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include <Game.Player.h>
#include <map>
namespace game::character
{
	void Docked::Reset(const game::Difficulty&)
	{

	}

	static const std::string FORMAT_DOCK = "You dock at {}!";

	static void SetAvatarStateToDocked()
	{
		auto avatar = data::game::Character::Read(Player::GetAvatarId()).value();
		avatar.state = (int)game::character::State::DOCK;
		data::game::Character::Write(Player::GetAvatarId(), avatar);
	}

	static std::optional<DockResult> DoDock(const common::XY<double>& location)
	{
		std::optional<DockResult> result = DockResult::DOCKED;
		game::Islands::AddVisit(
			location,
			game::character::Statistics::GetTurnsRemaining(game::Player::GetAvatarId()));
		game::islands::Quests::Update(location);
		if (game::character::Quest::Complete(location))
		{
			result = DockResult::COMPLETED_QUEST;
		}
		data::game::character::Dock::Write(Player::GetAvatarId(), location);
		SetAvatarStateToDocked();
		auto island = game::Islands::Read(location).value();
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
		return data::game::character::Dock::Read(Player::GetAvatarId());
	}
}