#include "Data.Game.Avatar.Dock.h"
#include "Data.Game.Island.DarkAlley.h"
#include <format>
#include <functional>
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include "Game.Messages.h"
#include <map>
#include "Visuals.Data.Colors.h"
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
		data::game::avatar::Dock::SetLocation(location, (int)game::avatar::DockedState::DOCK);
		auto island = game::Islands::Read(location).value();
		game::avatar::Log::Write({ visuals::data::Colors::GREEN, std::format(FORMAT_DOCK, island.name) });
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
			game::avatar::Log::Write({ visuals::data::Colors::GREEN, std::format(FORMAT_UNDOCK, island.name) });
			data::game::avatar::Dock::ClearLocation();
			return true;
		}
		return false;
	}

	std::optional<game::avatar::DockedState> GetDockedState()
	{
		auto state = data::game::avatar::Dock::GetState();
		if (state)
		{
			return (game::avatar::DockedState)state.value();
		}
		return std::nullopt;
	}

	static bool SetDockedState(const game::avatar::DockedState& dockedState)
	{
		auto location = GetDockedLocation();
		if (location)
		{
			data::game::avatar::Dock::SetLocation(location.value(), (int)dockedState);
		}
		return false;
	}

	struct DockedStateTransition
	{
		std::string logColor;
		std::string logText;
		avatar::DockedState dockedState;
	};

	static std::function<DockedStateTransition()> DoTransition(const DockedStateTransition& transition)
	{
		return [transition]() { return transition; };
	}

	static DockedStateTransition OnEnterDarkAlley()
	{
		auto location = GetDockedLocation().value();
		auto data = data::game::island::DarkAlley::Read(location).value();
		auto infamy = game::avatar::Statistics::GetInfamy();
		if (infamy < data.infamyRequirement)
		{
			game::Messages::Write(
				{
					"==FIGHT!==",
					{
						{
							{19,9},
							"You have been waylaid by a ruffian!",
							visuals::data::Colors::RED,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,11},
							"Prepare to fight!",
							visuals::data::Colors::RED,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
			return 
			{
				visuals::data::Colors::GREEN,
				"You enter dark alley.",
				avatar::DockedState::DARK_ALLEY_ENTRANCE
			};
		}
		return
		{
			visuals::data::Colors::GREEN,
			"You enter dark alley.",
			avatar::DockedState::DARK_ALLEY
		};
	}

	static DockedStateTransition OnDefeatRuffian()
	{
		return
		{
			visuals::data::Colors::GREEN,
			"You enter dark alley.",
			avatar::DockedState::DARK_ALLEY
		};
	}

	const std::map<avatar::DockedAction, std::map<avatar::DockedState, std::function<DockedStateTransition()>>> actionDescriptors =
	{
		{
			avatar::DockedAction::ENTER_MARKET,
			{
				{
					avatar::DockedState::MARKET_BUY,
					DoTransition(
						{
							visuals::data::Colors::GREEN,
							"You enter the market.",
							avatar::DockedState::MARKET
						})
				},
				{
					avatar::DockedState::MARKET_SELL,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You enter the market.",
						avatar::DockedState::MARKET
					})
				},
				{
					avatar::DockedState::DOCK,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You enter the market.",
						avatar::DockedState::MARKET
					})
				}
			}
		},
		{
			avatar::DockedAction::ENTER_DOCK,
			{
				{
					avatar::DockedState::MARKET,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You leave the market.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::SHIPYARD,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You leave the shipyard.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::JOB_BOARD,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You leave the job board.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::DARK_ALLEY_ENTRANCE,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You leave the dark alley.",
						avatar::DockedState::DOCK
					})
				},
			}
		},
		{
			avatar::DockedAction::MARKET_BUY,
			{
				{
					avatar::DockedState::MARKET,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You browse for items to buy.",
						avatar::DockedState::MARKET_BUY
					})
				}
			}
		},
		{
			avatar::DockedAction::MARKET_SELL,
			{
				{
					avatar::DockedState::MARKET,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You look to sell yer items.",
						avatar::DockedState::MARKET_SELL
					})
				}
			}
		},
		{
			avatar::DockedAction::ENTER_JOB_BOARD,
			{
				{
					avatar::DockedState::DOCK,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You check for posted jobs.",
						avatar::DockedState::JOB_BOARD
					})
				}
			}
		},
		{
			avatar::DockedAction::ENTER_SHIPYARD,
			{
				{
					avatar::DockedState::DOCK,
					DoTransition(
					{
						visuals::data::Colors::GREEN,
						"You enter the shipyard.",
						avatar::DockedState::SHIPYARD
					})
				}
			}
		},
		{
			avatar::DockedAction::ENTER_DARK_ALLEY,
			{
				{
					avatar::DockedState::DOCK,
					OnEnterDarkAlley
				}
			}
		},
		{
			avatar::DockedAction::DEFEAT_RUFFIAN,
			{
				{
					avatar::DockedState::DARK_ALLEY_ENTRANCE,
					OnDefeatRuffian
				}
			}
		}
	};

	bool DoActionTransition(const avatar::DockedAction& action)
	{
		auto dockedState = GetDockedState();
		if (dockedState)
		{
			auto descriptor = actionDescriptors.find(action);
			if (descriptor != actionDescriptors.end())
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

	//TODO: this seems no longer necessary...
	const std::map<avatar::DockedAction, std::function<bool(const avatar::DockedAction&)>> dockedActions =
	{
		{ avatar::DockedAction::UNDOCK, Undock},
		{ avatar::DockedAction::ENTER_MARKET, DoActionTransition},
		{ avatar::DockedAction::ENTER_DOCK, DoActionTransition},
		{ avatar::DockedAction::ENTER_JOB_BOARD, DoActionTransition},
		{ avatar::DockedAction::MARKET_BUY, DoActionTransition},
		{ avatar::DockedAction::MARKET_SELL,DoActionTransition},
		{ avatar::DockedAction::ENTER_SHIPYARD,DoActionTransition},
		{ avatar::DockedAction::ENTER_DARK_ALLEY,DoActionTransition},
		{ avatar::DockedAction::DEFEAT_RUFFIAN,DoActionTransition}
	};

	bool DoDockedAction(const avatar::DockedAction& action)
	{
		return dockedActions.find(action)->second(action);
	}

}