#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.Docks.h>
#include <format>
#include <functional>
#include "Game.Characters.Action.h"
#include "Game.Characters.Islands.h"
#include "Game.Characters.Items.h"
#include "Game.Characters.State.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Islands.h"
#include "Game.Session.h"
#include "Game.Ship.Docked.h"
namespace game
{
	static characters::State OnEnterDarkAlley(int characterId)
	{
		auto data = data::game::island::DarkAlley::Read(game::ship::Docked::GetIsland(data::game::character::Ship::ReadForCharacter(characterId).value().shipId).value()).value();
		auto infamy = game::Session().GetCharacters().GetCharacter(characterId).GetCounters().GetCounter(game::characters::Counter::INFAMY).GetValue();
		if (infamy < data.infamyRequirement)
		{
			return characters::State::DARK_ALLEY_ENTRANCE;
		}
		return characters::State::DARK_ALLEY;
	}

	static characters::State OnStartFishing(int characterId)
	{
		if (game::characters::Items::Has(characterId, Item::FISHING_POLE))
		{
			if (game::characters::Items::Has(characterId, Item::BAIT))
			{
				Fishboard::Generate(data::game::Player::GetCharacterId().value());
				return characters::State::FISHING;
			}
		}
		return characters::State::AT_SEA;
	}

	static std::function<characters::State(int)> DoTransition(const characters::State& state)
	{
		return [state](int) { return state; };
	}

	typedef std::function<characters::State(int)> Transitioner;
	typedef std::map<characters::State, Transitioner> ActionDescriptor;
	typedef std::map<characters::Action, ActionDescriptor> ActionDescriptorTable;

	static characters::State OnUndock(int characterId)
	{
		game::Session().GetCharacters().GetCharacter(characterId).ClearIsland();
		return characters::State::AT_SEA;
	}

	static characters::State OnDock(int characterId)
	{
		auto ship = data::game::character::Ship::ReadForCharacter(characterId).value();
		auto islandId = data::game::ship::Docks::Read(ship.shipId).value();
		game::Session().GetCharacters().GetCharacter(characterId).SetIsland(game::session::Island(islandId));
		return characters::State::DOCK;
	}

	const ActionDescriptorTable actionDescriptors =
	{
		{
			characters::Action::UNDOCK,
			{
				{
					characters::State::DOCK,
					OnUndock
				}
			}
		},
		{
			characters::Action::ENTER_MARKET,
			{
				{
					characters::State::MARKET_BUY,
					DoTransition(characters::State::MARKET)
				},
				{
					characters::State::MARKET_SELL,
					DoTransition(characters::State::MARKET)
				},
				{
					characters::State::DOCK,
					DoTransition(characters::State::MARKET)
				}
			}
		},
		{
			characters::Action::ENTER_DOCK,
			{
				{
					characters::State::AT_SEA,
					OnDock
				},
				{
					characters::State::TAVERN,
					DoTransition(characters::State::DOCK)
				},
				{
					characters::State::MARKET,
					DoTransition(characters::State::DOCK)
				},
				{
					characters::State::SHIPYARD,
					DoTransition(characters::State::DOCK)
				},
				{
					characters::State::JOB_BOARD,
					DoTransition(characters::State::DOCK)
				},
				{
					characters::State::DARK_ALLEY_ENTRANCE,
					DoTransition(characters::State::DOCK)
				},
				{
					characters::State::DARK_ALLEY,
					DoTransition(characters::State::DOCK)
				},
				{
					characters::State::TEMPLE,
					DoTransition(characters::State::DOCK)
				},
			}
		},
		{
			characters::Action::MARKET_BUY,
			{
				{
					characters::State::MARKET,
					DoTransition(characters::State::MARKET_BUY)
				}
			}
		},
		{
			characters::Action::MARKET_SELL,
			{
				{
					characters::State::MARKET,
					DoTransition(characters::State::MARKET_SELL)
				}
			}
		},
		{
			characters::Action::ENTER_JOB_BOARD,
			{
				{
					characters::State::DOCK,
					DoTransition(characters::State::JOB_BOARD)
				}
			}
		},
		{
			characters::Action::ENTER_TEMPLE,
			{
				{
					characters::State::DOCK,
					DoTransition(characters::State::TEMPLE)
				}
			}
		},
		{
			characters::Action::ENTER_TAVERN,
			{
				{
					characters::State::DOCK,
					DoTransition(characters::State::TAVERN)
				}
			}
		},
		{
			characters::Action::ENTER_SHIPYARD,
			{
				{
					characters::State::DOCK,
					DoTransition(characters::State::SHIPYARD)
				}
			}
		},
		{
			characters::Action::ENTER_DARK_ALLEY,
			{
				{
					characters::State::DOCK,
					OnEnterDarkAlley
				},
				{
					characters::State::GAMBLE_START,
					DoTransition(characters::State::DARK_ALLEY)
				}
			}
		},
		{
			characters::Action::DEFEAT_RUFFIAN,
			{
				{
					characters::State::DARK_ALLEY_ENTRANCE,
					DoTransition(characters::State::DARK_ALLEY)
				}
			}
		},
		{
			characters::Action::START_GAMBLING,
			{
				{
					characters::State::DARK_ALLEY,
					DoTransition(characters::State::GAMBLE_START)
				}
			}
		},
		{
			characters::Action::CAREEN_TO_PORT,
			{
				{
					characters::State::AT_SEA,
					DoTransition(characters::State::CAREENED_TO_PORT)
				}
			}
		},
		{
			characters::Action::CAREEN_TO_STARBOARD,
			{
				{
					characters::State::AT_SEA,
					DoTransition(characters::State::CAREENED_TO_STARBOARD)
				}
			}
		},
		{
			characters::Action::UNCAREEN,
			{
				{
					characters::State::CAREENED_TO_PORT,
					DoTransition(characters::State::AT_SEA)
				},
				{
					characters::State::CAREENED_TO_STARBOARD,
					DoTransition(characters::State::AT_SEA)
				}
			}
		},
		{
			characters::Action::START_FISHING,
			{
				{
					characters::State::AT_SEA,
					OnStartFishing
				}
			}
		},
		{
			characters::Action::STOP_FISHING,
			{
				{
					characters::State::FISHING,
					DoTransition(characters::State::AT_SEA)
				}
			}
		}
	};

	static const ActionDescriptorTable& GetActionDescriptors()
	{
		return actionDescriptors;
	}

	static std::optional<ActionDescriptor> FindActionDescriptor(const characters::Action& action)
	{
		auto descriptor = GetActionDescriptors().find(action);
		if (descriptor != GetActionDescriptors().end())
		{
			return descriptor->second;
		}
		return std::nullopt;
	}

	static std::optional<Transitioner> FindTransitioner(const ActionDescriptor& descriptor, const characters::State& state)
	{
		auto transition = descriptor.find(state);
		if (transition != descriptor.end())
		{
			return transition->second;
		}
		return std::nullopt;
	}
}