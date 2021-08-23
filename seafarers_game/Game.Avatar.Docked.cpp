#include <Data.Game.Avatar.Dock.h>
#include <Data.Game.Island.DarkAlley.h>
#include <format>
#include <functional>
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Docked.DockedStateTransition.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Colors.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include <map>
namespace game::avatar::Docked
{
	void Reset(const game::Difficulty&)
	{

	}

	const std::string FORMAT_DOCK = "You dock at {}!";

	static std::optional<DockResult> DoDock(const common::XY<double>& location)
	{
		std::optional<DockResult> result = DockResult::DOCKED;
		game::Islands::AddVisit(
			location,
			game::avatar::Statistics::GetTurnsRemaining());
		game::islands::Quests::Update(location);
		if (game::avatar::Quest::CompleteQuest(location))
		{
			result = DockResult::COMPLETED_QUEST;
		}
		data::game::avatar::Dock::SetLocation(location, (int)game::avatar::State::DOCK);
		auto island = game::Islands::Read(location).value();
		game::avatar::Log::Write({ game::Colors::GREEN, std::format(FORMAT_DOCK, island.name) });
		return result;
	}

	std::optional<DockResult> Dock()
	{
		if (GetDockedLocation().has_value())
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

	std::optional<common::XY<double>> GetDockedLocation()
	{
		return data::game::avatar::Dock::GetLocation();
	}

	const std::string FORMAT_UNDOCK = "You undock from {}.";

	static bool Undock(const avatar::DockedAction&)
	{
		auto location = GetDockedLocation();
		if (location.has_value())
		{
			auto island = game::Islands::Read(location.value()).value();
			game::avatar::Log::Write({ game::Colors::GREEN, std::format(FORMAT_UNDOCK, island.name) });
			data::game::avatar::Dock::ClearLocation();
			return true;
		}
		return false;
	}

	std::optional<game::avatar::State> GetState()
	{
		auto state = data::game::avatar::Dock::GetState();
		if (state)
		{
			return (game::avatar::State)state.value();
		}
		return std::nullopt;
	}

	static bool SetDockedState(const game::avatar::State& dockedState)
	{
		auto location = GetDockedLocation();
		if (location)
		{
			data::game::avatar::Dock::SetLocation(location.value(), (int)dockedState);
		}
		return false;
	}

	void OnEnterDarkAlleyFailsInfamyRequirement();

	DockedStateTransition OnEnterDarkAlley()
	{
		auto location = GetDockedLocation().value();
		auto data = data::game::island::DarkAlley::Read(location).value();
		auto infamy = game::avatar::Statistics::GetInfamy();
		if (infamy < data.infamyRequirement)
		{
			OnEnterDarkAlleyFailsInfamyRequirement();
			return 
			{
				game::Colors::GREEN,
				"You enter dark alley.",
				avatar::State::DARK_ALLEY_ENTRANCE
			};
		}
		return
		{
			game::Colors::GREEN,
			"You enter dark alley.",
			avatar::State::DARK_ALLEY
		};
	}

	DockedStateTransition OnDefeatRuffian()
	{
		//TODO: add a message
		return
		{
			game::Colors::GREEN,
			"You enter the dark alley.",
			avatar::State::DARK_ALLEY
		};
	}

	DockedStateTransition OnStartGambling()
	{
		//TODO: add a message
		return
		{
			game::Colors::GREEN,
			"You approach some shady characters playing a card game.",
			avatar::State::GAMBLE_START
		};
	}


	const std::map<avatar::DockedAction, std::map<avatar::State, std::function<DockedStateTransition()>>>& GetActionDescriptors();

	bool DoActionTransition(const avatar::DockedAction& action)
	{
		auto dockedState = GetState();
		if (dockedState)
		{
			auto descriptor = GetActionDescriptors().find(action);
			if (descriptor != GetActionDescriptors().end())
			{
				auto transition = descriptor->second.find(dockedState.value());
				{
					if (transition != descriptor->second.end())
					{
						auto result = transition->second();
						avatar::Log::Write({ result.logColor, result.logText });
						SetDockedState(result.dockedState);
					}
				}
			}
		}
		return false;
	}

	const std::map<avatar::DockedAction, std::function<bool(const avatar::DockedAction&)>> dockedActions =
	{
		{ avatar::DockedAction::UNDOCK, Undock}
	};

	bool DoDockedAction(const avatar::DockedAction& action)
	{
		auto iter = dockedActions.find(action);
		if (iter != dockedActions.end())
		{
			return iter->second(action);
		}
		return DoActionTransition(action);
	}

}