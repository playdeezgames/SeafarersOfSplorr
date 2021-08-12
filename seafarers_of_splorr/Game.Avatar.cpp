#include "UIState.h"
#include "Common.Data.h"
#include "Common.RNG.h"
#include "Common.Utility.h"
#include "Data.Game.Avatar.h"
#include "Data.Game.Avatar.Dock.h"
#include <format>
#include "Game.Avatar.h"
#include "Game.Avatar.DockedState.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Heading.h"
#include "Game.Islands.h"
#include "Game.Islands.Quests.h"
#include "Game.Ships.h"
#include "Game.World.h"
#include "Visuals.Data.Colors.h"
namespace game::Avatar
{
	void Reset(const game::Difficulty&)
	{
		auto worldSize = game::World::GetSize();
		data::game::Avatar::AvatarData data =
			{
				{worldSize.GetX()/2.0, worldSize.GetY()/2.0},
				common::Data::ModuloDouble(common::RNG::FromRange(0.0, game::Heading::DEGREES), game::Heading::DEGREES),
				1.0
			};
		data::game::Avatar::Write(data);
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
		game::avatar::Log::Write({visuals::data::Colors::GREEN, std::format(FORMAT_DOCK, island.name)});
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
			game::avatar::Log::Write({ visuals::data::Colors::GREEN, std::format(FORMAT_UNDOCK, island.name)  });
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

	const std::map<avatar::DockedAction, std::map<avatar::DockedState, DockedStateTransition>> actionDescriptors =
	{
		{
			avatar::DockedAction::ENTER_MARKET,
			{
				{
					avatar::DockedState::MARKET_BUY,
					{
						visuals::data::Colors::GREEN,
						"You enter the market.",
						avatar::DockedState::MARKET
					}
				},
				{
					avatar::DockedState::MARKET_SELL,
					{
						visuals::data::Colors::GREEN,
						"You enter the market.",
						avatar::DockedState::MARKET
					}
				},
				{
					avatar::DockedState::DOCK,
					{
						visuals::data::Colors::GREEN,
						"You enter the market.",
						avatar::DockedState::MARKET
					}
				}
			}
		},
		{
			avatar::DockedAction::ENTER_DOCK,
			{
				{
					avatar::DockedState::MARKET,
					{
						visuals::data::Colors::GREEN,
						"You leave the market.",
						avatar::DockedState::DOCK
					}
				},
				{
					avatar::DockedState::SHIPYARD,
					{
						visuals::data::Colors::GREEN,
						"You leave the shipyard.",
						avatar::DockedState::DOCK
					}
				},
				{
					avatar::DockedState::JOB_BOARD,
					{
						visuals::data::Colors::GREEN,
						"You leave the job board.",
						avatar::DockedState::DOCK
					}
				},
				{
					avatar::DockedState::DARK_ALLEY_ENTRANCE,
					{
						visuals::data::Colors::GREEN,
						"You leave the dark alley.",
						avatar::DockedState::DOCK
					}
				},
			}
		},
		{
			avatar::DockedAction::MARKET_BUY,
			{
				{
					avatar::DockedState::MARKET,
					{
						visuals::data::Colors::GREEN,
						"You browse for items to buy.",
						avatar::DockedState::MARKET_BUY
					}
				}
			}
		},
		{
			avatar::DockedAction::MARKET_SELL,
			{
				{
					avatar::DockedState::MARKET,
					{
						visuals::data::Colors::GREEN,
						"You look to sell yer items.",
						avatar::DockedState::MARKET_SELL
					}
				}
			}
		},
		{
			avatar::DockedAction::ENTER_JOB_BOARD,
			{
				{
					avatar::DockedState::DOCK,
					{
						visuals::data::Colors::GREEN,
						"You check for posted jobs.",
						avatar::DockedState::JOB_BOARD
					}
				}
			}
		},
		{
			avatar::DockedAction::ENTER_SHIPYARD,
			{
				{
					avatar::DockedState::DOCK,
					{
						visuals::data::Colors::GREEN,
						"You enter the shipyard.",
						avatar::DockedState::SHIPYARD
					}
				}
			}
		},
		{
			avatar::DockedAction::ENTER_DARK_ALLEY,
			{
				{
					avatar::DockedState::DOCK,
					{
						visuals::data::Colors::GREEN,
						"You enter dark alley.",
						avatar::DockedState::DARK_ALLEY_ENTRANCE
					}
				}
			}
		}
	};

	bool DoTransition(const avatar::DockedAction& action)
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
						avatar::Log::Write({transition->second.logColor, transition->second.logText});
						SetDockedState(transition->second.dockedState);
					}
				}
			}
		}
		return false;
	}

	const std::map<avatar::DockedAction, std::function<bool(const avatar::DockedAction&)>> dockedActions =
	{
		{ avatar::DockedAction::UNDOCK, Undock},
		{ avatar::DockedAction::ENTER_MARKET, DoTransition},
		{ avatar::DockedAction::ENTER_DOCK, DoTransition},
		{ avatar::DockedAction::ENTER_JOB_BOARD, DoTransition},
		{ avatar::DockedAction::MARKET_BUY, DoTransition},
		{ avatar::DockedAction::MARKET_SELL,DoTransition},
		{ avatar::DockedAction::ENTER_SHIPYARD,DoTransition},
		{ avatar::DockedAction::ENTER_DARK_ALLEY,DoTransition}
	};

	bool DoDockedAction(const avatar::DockedAction& action)
	{
		return dockedActions.find(action)->second(action);
	}

}