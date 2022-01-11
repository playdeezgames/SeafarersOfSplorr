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
	static std::function<characters::State(int)> DoTransition(const characters::State& state)
	{
		return [state](int) { return state; };
	}

	typedef std::function<characters::State(int)> Transitioner;
	typedef std::map<characters::State, Transitioner> ActionDescriptor;
	typedef std::map<characters::Action, ActionDescriptor> ActionDescriptorTable;

	static characters::State OnUndock(int characterId)
	{
		game::Session()
			.GetCharacters()
			.GetCharacter(characterId)
			.ClearIsland();
		return characters::State::AT_SEA;
	}

	static characters::State OnDock(int characterId)
	{
		auto character = Characters().GetCharacter(characterId);
		character.SetIsland(character.GetBerth().GetShip().GetIsland());
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

	static std::optional<ActionDescriptor> FindActionDescriptor(const characters::Action& action)
	{
		auto descriptor = actionDescriptors.find(action);
		if (descriptor != actionDescriptors.end())
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

	bool Character::IsDead() const
	{
		return GetHitpoints().GetCurrent() <= 0;
	}

	void Character::SetIsland(Island island) const
	{
		data::game::character::CurrentIsland::Write(characterId, (int)island);
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

	static size_t DetermineTurnsSpent(int characterId)
	{
		auto plights = Character::ToCharacter(characterId).GetPlights();
		size_t agingRate = 1;
		if (plights.Has(characters::Plight::DOUBLE_AGING) &&
			!plights.Has(characters::Plight::AGING_IMMUNITY))
		{
			agingRate = 2;
		}
		if (plights.Has(characters::Plight::AGING_IMMUNITY) &&
			!plights.Has(characters::Plight::DOUBLE_AGING))
		{
			agingRate = 0;
		}
		return agingRate;
	}

	static void ApplyTurn(int characterId)
	{
		auto turnsSpent = DetermineTurnsSpent(characterId);
		while (turnsSpent)
		{
			Characters()
				.GetCharacter(characterId)
				.GetCounters()
				.GetCounter(game::characters::Counter::TURNS_REMAINING)
				.Change(-1);
			turnsSpent--;
		}
	}

	static void SufferWoundDueToStarvation(int characterId)
	{
		auto counters =
			Characters()
			.GetCharacter(characterId)
			.GetCounters();
		counters
			.GetCounter(game::characters::Counter::STARVATION)
			.Reset();
		counters
			.GetCounter(game::characters::Counter::WOUNDS)
			.Change(1);
	}

	static void SufferStarvationDueToHunger(int characterId)
	{
		Characters()
			.GetCharacter(characterId)
			.GetCharacteristics()
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnOpposedCheck(
				Characters().GetCharacter(characterId).GetCounters().GetCounter(game::characters::Counter::STARVATION).Change(1),
				[characterId](bool success)
				{
					if (!success)
					{
						SufferWoundDueToStarvation(characterId);
					}
				});
	}

	static void SufferHunger(int characterId)
	{
		Characters()
			.GetCharacter(characterId)
			.GetCharacteristics()
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnOpposedCheck(
				Characters().GetCharacter(characterId).GetCounters().GetCounter(game::characters::Counter::STARVATION).Change(1),
				[characterId](bool success)
				{
					if (!success)
					{
						SufferStarvationDueToHunger(characterId);
					}
				});
	}

	void Character::ApplyTurnEffects() const
	{
		SufferHunger(characterId);
		ApplyTurn(characterId);
		GetPlights().ApplyTurnEffects();
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

	bool Character::IsOnIsland() const
	{
		return TryGetIsland().has_value();
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