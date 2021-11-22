#include <Data.Game.Character.h>
#include <Data.Game.Character.Dock.h>
#include <Data.Game.Island.DarkAlley.h>
#include <format>
#include <functional>
#include "Game.Character.Action.h"
#include "Game.Character.Actions.h"
#include "Game.Character.Docked.h"
#include "Game.Avatar.Items.h"
#include "Game.Character.State.h"
#include "Game.Character.StateTransition.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Islands.h"
#include <map>
namespace game::character
{
	const std::string FORMAT_UNDOCK = "You undock from {}.";

	static StateTransition OnUndock(int avatarId)
	{
		auto location = game::character::Docked::ReadLocation();
		auto island = game::Islands::Read(location.value()).value();
		data::game::character::Dock::Clear(avatarId);
		return {
			game::Colors::GREEN,
			std::format(FORMAT_UNDOCK, island.name),
			State::AT_SEA
		};
	}

	void OnEnterDarkAlleyFailsInfamyRequirement();//this is outside of the game, and needs reference to visuals

	static StateTransition OnEnterDarkAlley(int avatarId)
	{
		auto location = game::character::Docked::ReadLocation().value();
		auto data = data::game::island::DarkAlley::Read(location).value();
		auto infamy = game::avatar::Statistics::GetInfamy(avatarId);
		if (infamy < data.infamyRequirement)
		{
			OnEnterDarkAlleyFailsInfamyRequirement();
			return
			{
				game::Colors::GREEN,
				"You enter dark alley.",
				State::DARK_ALLEY_ENTRANCE
			};
		}
		return
		{
			game::Colors::GREEN,
			"You enter dark alley.",
			State::DARK_ALLEY
		};
	}

	static StateTransition OnDefeatRuffian(int)
	{
		//TODO: add a message
		return
		{
			game::Colors::GREEN,
			"You enter the dark alley.",
			State::DARK_ALLEY
		};
	}

	static StateTransition OnStartGambling(int)
	{
		//TODO: add a message
		return
		{
			game::Colors::GREEN,
			"You approach some shady characters playing a card game.",
			State::GAMBLE_START
		};
	}

	static StateTransition OnStartFishing(int avatarId)
	{
		if (game::avatar::Items::Has(avatarId, Item::FISHING_POLE))
		{
			if (game::avatar::Items::Has(avatarId, Item::BAIT))
			{
				Fishboard::Generate();
				return
				{
					game::Colors::GRAY,
					"Nice day for fishin', innit? Huh-ha!",
					State::FISHING
				};
			}
		}
		return
		{
			game::Colors::RED,
			"You cannot fish right now.",
			State::AT_SEA
		};
	}

	static StateTransition OnStopFishing(int)
	{
		return
		{
			game::Colors::GRAY,
			"You wake from yer nap.",
			State::AT_SEA
		};
	}

	static std::function<StateTransition(int)> DoTransition(const StateTransition& transition)
	{
		return [transition](int) { return transition; };
	}

	const std::map<Action, std::map<State, std::function<StateTransition(int)>>> actionDescriptors =
	{
		{
			Action::UNDOCK,
			{
				{
					State::DOCK,
					OnUndock
				}
			}
		},
		{
			Action::ENTER_MARKET,
			{
				{
					State::MARKET_BUY,
					DoTransition(
						{
							game::Colors::GREEN,
							"You enter the market.",
							State::MARKET
						})
				},
				{
					State::MARKET_SELL,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						State::MARKET
					})
				},
				{
					State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						State::MARKET
					})
				}
			}
		},
		{
			Action::ENTER_DOCK,
			{
				{
					State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the market.",
						State::DOCK
					})
				},
				{
					State::SHIPYARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the shipyard.",
						State::DOCK
					})
				},
				{
					State::JOB_BOARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the job board.",
						State::DOCK
					})
				},
				{
					State::DARK_ALLEY_ENTRANCE,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						State::DOCK
					})
				},
				{
					State::DARK_ALLEY,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						State::DOCK
					})
				},
				{
					State::TEMPLE,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the temple.",
						State::DOCK
					})
				},
			}
		},
		{
			Action::MARKET_BUY,
			{
				{
					State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You browse for items to buy.",
						State::MARKET_BUY
					})
				}
			}
		},
		{
			Action::MARKET_SELL,
			{
				{
					State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You look to sell yer items.",
						State::MARKET_SELL
					})
				}
			}
		},
		{
			Action::ENTER_JOB_BOARD,
			{
				{
					State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You check for posted jobs.",
						State::JOB_BOARD
					})
				}
			}
		},
		{
			Action::ENTER_TEMPLE,
			{
				{
					State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the temple.",
						State::TEMPLE
					})
				}
			}
		},
		{
			Action::ENTER_SHIPYARD,
			{
				{
					State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the shipyard.",
						State::SHIPYARD
					})
				}
			}
		},
		{
			Action::ENTER_DARK_ALLEY,
			{
				{
					State::DOCK,
					OnEnterDarkAlley
				},
				{
					State::GAMBLE_START,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the game.",
						State::DARK_ALLEY
					})
				}
			}
		},
		{
			Action::DEFEAT_RUFFIAN,
			{
				{
					State::DARK_ALLEY_ENTRANCE,
					OnDefeatRuffian
				}
			}
		},
		{
			Action::START_GAMBLING,
			{
				{
					State::DARK_ALLEY,
					OnStartGambling
				}
			}
		},
		{
			Action::CAREEN_TO_PORT,
			{
				{
					State::AT_SEA,
					DoTransition({
						game::Colors::GREEN,
						"You heave down to port.",
						State::CAREENED_TO_PORT
					})
				}
			}
		},
		{
			Action::CAREEN_TO_STARBOARD,
			{
				{
					State::AT_SEA,
					DoTransition({
						game::Colors::GREEN,
						"You heave down to starboard.",
						State::CAREENED_TO_STARBOARD
					})
				}
			}
		},
		{
			Action::UNCAREEN,
			{
				{
					State::CAREENED_TO_PORT,
					DoTransition({
						game::Colors::GREEN,
						"You right the vessel.",
						State::AT_SEA
					})
				},
				{
					State::CAREENED_TO_STARBOARD,
					DoTransition({
						game::Colors::GREEN,
						"You right the vessel.",
						State::AT_SEA
					})
				}
			}
		},
		{
			Action::START_FISHING,
			{
				{
					State::AT_SEA,
					OnStartFishing
				}
			}
		},
		{
			Action::STOP_FISHING,
			{
				{
					State::FISHING,
					OnStopFishing
				}
			}
		}
	};

	static const std::map<Action, std::map<State, std::function<StateTransition(int)>>>& GetActionDescriptors()
	{
		return actionDescriptors;
	}

	static void SetState(int avatarId, const State& state)
	{
		auto avatar = data::game::Character::Read(avatarId).value();
		avatar.state = (int)state;
		data::game::Character::Write(avatarId, avatar);
	}

	std::optional<State> Actions::GetState(int avatarId)
	{
		auto avatar = data::game::Character::Read(avatarId);
		if (avatar)
		{
			return (State)avatar.value().state;
		}
		return std::nullopt;
	}

	bool Actions::DoAction(int avatarId, const Action& action)
	{
		auto state = GetState(avatarId);
		if (state)
		{
			auto descriptor = GetActionDescriptors().find(action);
			if (descriptor != GetActionDescriptors().end())
			{
				auto transition = descriptor->second.find(state.value());
				{
					if (transition != descriptor->second.end())
					{
						auto result = transition->second(avatarId);
						SetState(avatarId, result.state);
					}
				}
			}
		}
		return false;
	}


}