#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Ship.Docks.h>
#include <format>
#include <functional>
#include "Game.Character.h"
#include "Game.Character.Action.h"
#include "Game.Character.Actions.h"
#include "Game.Character.Items.h"
#include "Game.Character.State.h"
#include "Game.Character.StateTransition.h"
#include "Game.Character.Statistics.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Islands.h"
#include "Game.Player.h"
#include "Game.Ship.Docked.h"
namespace game::character
{
	const std::string FORMAT_UNDOCK = "You undock from {}.";

	static StateTransition OnUndock(int characterId)
	{
		auto islandId = game::ship::Docked::GetIsland(data::game::character::Ship::ReadForCharacter(characterId).value().shipId).value();
		auto island = game::Islands::Read(islandId).value();
		data::game::ship::Docks::Clear(data::game::character::Ship::ReadForCharacter(characterId).value().shipId);
		return {
			game::Colors::GREEN,
			std::format(FORMAT_UNDOCK, island.name),
			State::AT_SEA
		};
	}

	void OnEnterDarkAlleyFailsInfamyRequirement();//this is outside of the game, and needs reference to visuals

	static StateTransition OnEnterDarkAlley(int characterId)
	{
		auto data = data::game::island::DarkAlley::Read(game::ship::Docked::GetIsland(data::game::character::Ship::ReadForCharacter(characterId).value().shipId).value()).value();
		auto infamy = game::character::Statistics::GetInfamy(characterId);
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

	static StateTransition OnStartFishing(int characterId)
	{
		if (game::character::Items::Has(characterId, Item::FISHING_POLE))
		{
			if (game::character::Items::Has(characterId, Item::BAIT))
			{
				Fishboard::Generate(game::Player::GetCharacterId());
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

	typedef std::function<StateTransition(int)> Transitioner;
	typedef std::map<State, Transitioner> ActionDescriptor;
	typedef std::map<Action, ActionDescriptor> ActionDescriptorTable;

	const ActionDescriptorTable actionDescriptors =
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

	static const ActionDescriptorTable& GetActionDescriptors()
	{
		return actionDescriptors;
	}

	static std::optional<ActionDescriptor> FindActionDescriptor(const Action& action)
	{
		auto descriptor = GetActionDescriptors().find(action);
		if (descriptor != GetActionDescriptors().end())
		{
			return descriptor->second;
		}
		return std::nullopt;
	}

	static std::optional<Transitioner> FindTransitioner(const ActionDescriptor& descriptor, const State& state)
	{
		auto transition = descriptor.find(state);
		if (transition != descriptor.end())
		{
			return transition->second;
		}
		return std::nullopt;
	}

	void Actions::DoAction(int characterId, const Action& action)
	{
		auto state = Character::GetState(characterId);
		if (state)
		{
			auto descriptor = FindActionDescriptor(action);
			if (descriptor)
			{
				auto transitioner = FindTransitioner(descriptor.value(), state.value());
				if (transitioner)
				{
					Character::SetState(characterId, transitioner.value()(characterId).state);
				}
			}
		}
	}
}