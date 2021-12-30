#include <Data.Game.Character.h>
#include <Data.Game.Character.CurrentIsland.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.CurrentIsland.h>
#include "Game.Session.h"
#include "Game.Session.Character.h"
#include <map>
namespace game::session
{
	character::HitPoints Character::GetHitpoints() const
	{
		return character::HitPoints(characterId);
	}

	static std::optional<character::Berth> TryGetBerth(int characterId)
	{
		if (data::game::character::Ship::ReadShipForCharacter(characterId).has_value())
		{
			return character::Berth(characterId);
		}
		return std::nullopt;
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
		auto shipId = data::game::character::Ship::ReadShipForCharacter(characterId).value();
		auto islandId = data::game::ship::CurrentIsland::Read(shipId).value();
		Characters().GetCharacter(characterId).SetIsland(Island(islandId));
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
			characters::Action::ENTER_DOCK,
			{
				{
					characters::State::AT_SEA,
					OnDock
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

	void Character::DoAction(const game::characters::Action& action) const
	{
		auto state = TryGetState();
		if (state)
		{
			auto descriptor = FindActionDescriptor(action);
			if (descriptor)
			{
				auto transitioner = FindTransitioner(descriptor.value(), state.value());
				if (transitioner)
				{
					data::game::Character::WriteState(
						characterId, 
						(int)transitioner.value()(characterId));
				}
			}
		}
	}

	character::Messages Character::GetMessages() const
	{
		return character::Messages(characterId);
	}

	character::Berth Character::GetBerth() const
	{
		return TryGetBerth(characterId).value();
	}

	bool Character::IsDead() const
	{
		return GetHitpoints().GetCurrent() <= 0;
	}

	character::Items Character::GetItems() const
	{
		return character::Items(characterId);
	}

	character::Characteristics Character::GetCharacteristics() const
	{
		return character::Characteristics(characterId);
	}

	void Character::SetIsland(Island island) const
	{
		data::game::character::CurrentIsland::Write(characterId, (int)island);
	}

	character::Counters Character::GetCounters() const
	{
		return character::Counters(characterId);
	}

	character::Flags Character::GetFlags() const
	{
		return character::Flags(characterId);
	}

	std::string Character::GetName() const
	{
		return data::game::Character::ReadName(characterId).value();
	}

	std::optional<characters::State> Character::TryGetState() const
	{
		auto state = data::game::Character::ReadState(characterId);
		if (state)
		{
			return (characters::State)state.value();
		}
		return std::nullopt;
	}

	void Character::ApplyTurnEffects() const
	{
		GetPlights().ApplyTurnEffects();
	}

	characters::State Character::GetState() const
	{
		return TryGetState().value();
	}

	void Character::ClearIsland() const
	{
		data::game::character::CurrentIsland::Clear(characterId);
	}

	std::optional<Island> Character::TryGetIsland() const
	{
		auto islandId = data::game::character::CurrentIsland::Read(characterId);
		if (islandId)
		{
			return Island(islandId.value());
		}
		return std::nullopt;
	}

	Island Character::GetIsland() const
	{
		return TryGetIsland().value();
	}

	Island Character::GetOriginIsland() const
	{
		return Island(data::game::Character::ReadOriginIslandId(characterId).value());
	}

	Tribe Character::GetTribe() const
	{
		return Tribe(data::game::Character::ReadTribeId(characterId).value());
	}

	bool Character::IsPlayer() const
	{
		return data::game::Player::GetCharacterId() == characterId;
	}

}