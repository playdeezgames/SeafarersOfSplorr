#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Ship.Docks.h>
#include <format>
#include <functional>
#include "Game.Character.h"
#include "Game.Character.Action.h"
#include "Game.Character.Island.h"
#include "Game.Character.Items.h"
#include "Game.Character.State.h"
#include "Game.Character.Statistics.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Islands.h"
#include "Game.Player.h"
#include "Game.Ship.Docked.h"
namespace game
{
	static character::State OnEnterDarkAlley(int characterId)
	{
		auto data = data::game::island::DarkAlley::Read(game::ship::Docked::GetIsland(data::game::character::Ship::ReadForCharacter(characterId).value().shipId).value()).value();
		auto infamy = game::character::statistics::Infamy::Current(characterId).value();
		if (infamy < data.infamyRequirement)
		{
			return character::State::DARK_ALLEY_ENTRANCE;
		}
		return character::State::DARK_ALLEY;
	}

	static character::State OnStartFishing(int characterId)
	{
		if (game::character::Items::Has(characterId, Item::FISHING_POLE))
		{
			if (game::character::Items::Has(characterId, Item::BAIT))
			{
				Fishboard::Generate(game::Player::GetCharacterId());
				return character::State::FISHING;
			}
		}
		return character::State::AT_SEA;
	}

	static std::function<character::State(int)> DoTransition(const character::State& state)
	{
		return [state](int) { return state; };
	}

	typedef std::function<character::State(int)> Transitioner;
	typedef std::map<character::State, Transitioner> ActionDescriptor;
	typedef std::map<character::Action, ActionDescriptor> ActionDescriptorTable;

	static character::State OnUndock(int characterId)
	{
		game::character::Island::Clear(characterId);
		return character::State::AT_SEA;
	}

	static character::State OnDock(int characterId)
	{
		auto ship = data::game::character::Ship::ReadForCharacter(characterId).value();
		auto islandId = data::game::ship::Docks::Read(ship.shipId).value();
		game::character::Island::Write(characterId, islandId);
		return character::State::DOCK;
	}

	const ActionDescriptorTable actionDescriptors =
	{
		{
			character::Action::UNDOCK,
			{
				{
					character::State::DOCK,
					OnUndock
				}
			}
		},
		{
			character::Action::ENTER_MARKET,
			{
				{
					character::State::MARKET_BUY,
					DoTransition(character::State::MARKET)
				},
				{
					character::State::MARKET_SELL,
					DoTransition(character::State::MARKET)
				},
				{
					character::State::DOCK,
					DoTransition(character::State::MARKET)
				}
			}
		},
		{
			character::Action::ENTER_DOCK,
			{
				{
					character::State::AT_SEA,
					OnDock
				},
				{
					character::State::TAVERN,
					DoTransition(character::State::DOCK)
				},
				{
					character::State::MARKET,
					DoTransition(character::State::DOCK)
				},
				{
					character::State::SHIPYARD,
					DoTransition(character::State::DOCK)
				},
				{
					character::State::JOB_BOARD,
					DoTransition(character::State::DOCK)
				},
				{
					character::State::DARK_ALLEY_ENTRANCE,
					DoTransition(character::State::DOCK)
				},
				{
					character::State::DARK_ALLEY,
					DoTransition(character::State::DOCK)
				},
				{
					character::State::TEMPLE,
					DoTransition(character::State::DOCK)
				},
			}
		},
		{
			character::Action::MARKET_BUY,
			{
				{
					character::State::MARKET,
					DoTransition(character::State::MARKET_BUY)
				}
			}
		},
		{
			character::Action::MARKET_SELL,
			{
				{
					character::State::MARKET,
					DoTransition(character::State::MARKET_SELL)
				}
			}
		},
		{
			character::Action::ENTER_JOB_BOARD,
			{
				{
					character::State::DOCK,
					DoTransition(character::State::JOB_BOARD)
				}
			}
		},
		{
			character::Action::ENTER_TEMPLE,
			{
				{
					character::State::DOCK,
					DoTransition(character::State::TEMPLE)
				}
			}
		},
		{
			character::Action::ENTER_TAVERN,
			{
				{
					character::State::DOCK,
					DoTransition(character::State::TAVERN)
				}
			}
		},
		{
			character::Action::ENTER_SHIPYARD,
			{
				{
					character::State::DOCK,
					DoTransition(character::State::SHIPYARD)
				}
			}
		},
		{
			character::Action::ENTER_DARK_ALLEY,
			{
				{
					character::State::DOCK,
					OnEnterDarkAlley
				},
				{
					character::State::GAMBLE_START,
					DoTransition(character::State::DARK_ALLEY)
				}
			}
		},
		{
			character::Action::DEFEAT_RUFFIAN,
			{
				{
					character::State::DARK_ALLEY_ENTRANCE,
					DoTransition(character::State::DARK_ALLEY)
				}
			}
		},
		{
			character::Action::START_GAMBLING,
			{
				{
					character::State::DARK_ALLEY,
					DoTransition(character::State::GAMBLE_START)
				}
			}
		},
		{
			character::Action::CAREEN_TO_PORT,
			{
				{
					character::State::AT_SEA,
					DoTransition(character::State::CAREENED_TO_PORT)
				}
			}
		},
		{
			character::Action::CAREEN_TO_STARBOARD,
			{
				{
					character::State::AT_SEA,
					DoTransition(character::State::CAREENED_TO_STARBOARD)
				}
			}
		},
		{
			character::Action::UNCAREEN,
			{
				{
					character::State::CAREENED_TO_PORT,
					DoTransition(character::State::AT_SEA)
				},
				{
					character::State::CAREENED_TO_STARBOARD,
					DoTransition(character::State::AT_SEA)
				}
			}
		},
		{
			character::Action::START_FISHING,
			{
				{
					character::State::AT_SEA,
					OnStartFishing
				}
			}
		},
		{
			character::Action::STOP_FISHING,
			{
				{
					character::State::FISHING,
					DoTransition(character::State::AT_SEA)
				}
			}
		}
	};

	static const ActionDescriptorTable& GetActionDescriptors()
	{
		return actionDescriptors;
	}

	static std::optional<ActionDescriptor> FindActionDescriptor(const character::Action& action)
	{
		auto descriptor = GetActionDescriptors().find(action);
		if (descriptor != GetActionDescriptors().end())
		{
			return descriptor->second;
		}
		return std::nullopt;
	}

	static std::optional<Transitioner> FindTransitioner(const ActionDescriptor& descriptor, const character::State& state)
	{
		auto transition = descriptor.find(state);
		if (transition != descriptor.end())
		{
			return transition->second;
		}
		return std::nullopt;
	}

	void Character::DoAction(int characterId, const character::Action& action)
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
					Character::SetState(characterId, transitioner.value()(characterId));
				}
			}
		}
	}
}