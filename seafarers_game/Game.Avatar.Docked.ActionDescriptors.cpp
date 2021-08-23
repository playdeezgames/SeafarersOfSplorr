#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
#include "Game.Avatar.StateTransition.h"
#include "Game.Colors.h"
#include <functional>
#include <map>
namespace game::avatar::Docked
{
	StateTransition OnEnterDarkAlley();
	StateTransition OnDefeatRuffian();
	StateTransition OnStartGambling();

	static std::function<StateTransition()> DoTransition(const StateTransition& transition)
	{
		return [transition]() { return transition; };
	}

	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>> actionDescriptors =
	{
		{
			avatar::Action::ENTER_MARKET,
			{
				{
					avatar::State::MARKET_BUY,
					DoTransition(
						{
							game::Colors::GREEN,
							"You enter the market.",
							avatar::State::MARKET
						})
				},
				{
					avatar::State::MARKET_SELL,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						avatar::State::MARKET
					})
				},
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						avatar::State::MARKET
					})
				}
			}
		},
		{
			avatar::Action::ENTER_DOCK,
			{
				{
					avatar::State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the market.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::SHIPYARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the shipyard.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::JOB_BOARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the job board.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::DARK_ALLEY_ENTRANCE,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::DARK_ALLEY,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						avatar::State::DOCK
					})
				},
			}
		},
		{
			avatar::Action::MARKET_BUY,
			{
				{
					avatar::State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You browse for items to buy.",
						avatar::State::MARKET_BUY
					})
				}
			}
		},
		{
			avatar::Action::MARKET_SELL,
			{
				{
					avatar::State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You look to sell yer items.",
						avatar::State::MARKET_SELL
					})
				}
			}
		},
		{
			avatar::Action::ENTER_JOB_BOARD,
			{
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You check for posted jobs.",
						avatar::State::JOB_BOARD
					})
				}
			}
		},
		{
			avatar::Action::ENTER_SHIPYARD,
			{
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the shipyard.",
						avatar::State::SHIPYARD
					})
				}
			}
		},
		{
			avatar::Action::ENTER_DARK_ALLEY,
			{
				{
					avatar::State::DOCK,
					OnEnterDarkAlley
				},
				{
					avatar::State::GAMBLE_START,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the game.",
						avatar::State::DARK_ALLEY
					})
				}
			}
		},
		{
			avatar::Action::DEFEAT_RUFFIAN,
			{
				{
					avatar::State::DARK_ALLEY_ENTRANCE,
					OnDefeatRuffian
				}
			}
		},
		{
			avatar::Action::START_GAMBLING,
			{
				{
					avatar::State::DARK_ALLEY,
					OnStartGambling
				}
			}
		}
	};

	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>>& GetActionDescriptors()
	{
		return actionDescriptors;
	}

}