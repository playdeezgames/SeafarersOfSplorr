#include "Game.Avatar.DockedAction.h"
#include "Game.Avatar.DockedState.h"
#include "Game.Avatar.Docked.DockedStateTransition.h"
#include <map>
#include <functional>
#include "Game.Colors.h"
namespace game::avatar::Docked
{
	DockedStateTransition OnEnterDarkAlley();
	DockedStateTransition OnDefeatRuffian();

	static std::function<DockedStateTransition()> DoTransition(const DockedStateTransition& transition)
	{
		return [transition]() { return transition; };
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
							game::Colors::GREEN,
							"You enter the market.",
							avatar::DockedState::MARKET
						})
				},
				{
					avatar::DockedState::MARKET_SELL,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						avatar::DockedState::MARKET
					})
				},
				{
					avatar::DockedState::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
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
						game::Colors::GREEN,
						"You leave the market.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::SHIPYARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the shipyard.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::JOB_BOARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the job board.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::DARK_ALLEY_ENTRANCE,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						avatar::DockedState::DOCK
					})
				},
				{
					avatar::DockedState::DARK_ALLEY,
					DoTransition(
					{
						game::Colors::GREEN,
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
						game::Colors::GREEN,
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
						game::Colors::GREEN,
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
						game::Colors::GREEN,
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
						game::Colors::GREEN,
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
		},
		{
			avatar::DockedAction::START_GAMBLING,
			{
				{
					avatar::DockedState::DARK_ALLEY,
					DoTransition(
					{
						game::Colors::GREEN,
						"You approach some shady characters playing a card game.",
						avatar::DockedState::DARK_ALLEY_GAMBLING
					})
				}
			}
		}
	};

	const std::map<avatar::DockedAction, std::map<avatar::DockedState, std::function<DockedStateTransition()>>>& GetActionDescriptors()
	{
		return actionDescriptors;
	}

}