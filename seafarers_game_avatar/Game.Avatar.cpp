#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.h>
#include <Data.Game.Avatar.Rations.h>
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Plights.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.StateTransition.h"
#include <Game.Items.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include <Game.ShipNames.h>
#include <Game.ShipTypes.h>
#include <Game.World.h>
#include <map>
namespace game::avatar
{
	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>>& GetActionDescriptors();
}
namespace game
{
	static void SetState(const game::avatar::State& state)
	{
		data::game::Avatar::WriteState(Player::GetAvatarId(), (int)state);
	}


	bool Avatar::DoAction(const avatar::Action& action)
	{
		auto state = GetState();
		if (state)
		{
			auto descriptor = avatar::GetActionDescriptors().find(action);
			if (descriptor != avatar::GetActionDescriptors().end())
			{
				auto transition = descriptor->second.find(state.value());
				{
					if (transition != descriptor->second.end())
					{
						auto result = transition->second();
						avatar::Log::Write({ result.logColor, result.logText });
						SetState(result.dockedState);
					}
				}
			}
		}
		return false;
	}

	std::optional<game::avatar::State> Avatar::GetState()
	{
		auto state = data::game::Avatar::ReadState(Player::GetAvatarId());
		if (state)
		{
			return (game::avatar::State)state.value();
		}
		return std::nullopt;
	}
	static void ApplyHunger()
	{
		double delta = -1.0;
		if (avatar::Plights::Has(avatar::Plight::DOUBLE_HUNGER) && !avatar::Plights::Has(avatar::Plight::HUNGER_IMMUNITY))
		{
			delta *= 2.0;
		}
		if (avatar::Plights::Has(avatar::Plight::HUNGER_IMMUNITY) && !avatar::Plights::Has(avatar::Plight::DOUBLE_HUNGER))
		{
			delta = 0.0;
		}
		if (!game::avatar::Statistics::IsStarving())
		{
			game::avatar::Statistics::ChangeSatiety(delta);
		}
		else
		{
			game::avatar::Statistics::ChangeHealth(delta);
		}
	}

	static void ApplyEating()
	{
		const double EAT_BENEFIT = 10.0;
		if (game::avatar::Statistics::NeedToEat(EAT_BENEFIT))
		{
			auto rations = game::avatar::Items::Read(game::Item::RATIONS);
			if (rations > 0)
			{
				game::avatar::Statistics::Eat(EAT_BENEFIT);
				game::avatar::Items::Remove(game::Item::RATIONS, 1);
			}
		}
	}

	static void ApplyTurn()
	{
		if (avatar::Plights::Has(avatar::Plight::DOUBLE_AGING) && !avatar::Plights::Has(avatar::Plight::AGING_IMMUNITY))
		{
			game::avatar::Statistics::SpendTurn();
			game::avatar::Statistics::SpendTurn();
			return;
		}
		if (avatar::Plights::Has(avatar::Plight::AGING_IMMUNITY) && !avatar::Plights::Has(avatar::Plight::DOUBLE_AGING))
		{
			//do nothing!
			return;
		}
		game::avatar::Statistics::SpendTurn();
	}

	void Avatar::ApplyTurnEffects()
	{
		ApplyTurn();
		ApplyHunger();
		ApplyEating();
	}

	void Avatar::Reset(const game::Difficulty&)
	{
		data::game::Avatar data =
		{
			0,
			"nada"//TODO: generate a name?
		};
		data::game::Avatar::Write(Player::GetAvatarId(), data);
		data::game::Avatar::WriteState(Player::GetAvatarId(), (int)game::avatar::State::AT_SEA);

		data::game::avatar::Rations::Write(Player::GetAvatarId(), (int)game::Items::GenerateRations());

		auto worldSize = game::World::GetSize();
		auto shipType = game::ShipTypes::GenerateForAvatar();
		int shipId = game::Ship::Add(
			shipType, 
			game::ShipNames::Generate(), 
			{ 
				worldSize.GetX() / 2.0, 
				worldSize.GetY() / 2.0 
			}, 
			common::Data::ModuloDouble(
				common::RNG::FromRange(0.0, common::Heading::DEGREES), 
				common::Heading::DEGREES).value(), 
			1.0);
		game::avatar::Ship::Write({ shipId, BerthType::CAPTAIN });
	}

	std::optional<game::Item> Avatar::GetRations()
	{
		auto itemId = data::game::avatar::Rations::Read(Player::GetAvatarId());
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Avatar::SetRations(const std::optional<game::Item>& item)
	{
		data::game::avatar::Rations::Clear(Player::GetAvatarId());
		if (item)
		{
			data::game::avatar::Rations::Write(Player::GetAvatarId(), (int)item.value());
		}
	}

	std::optional<std::string> Avatar::GetName(int avatarId)
	{
		auto avatar = data::game::Avatar::Read(avatarId);
		if (avatar)
		{
			return avatar.value().name;
		}
		return std::nullopt;
	}

}