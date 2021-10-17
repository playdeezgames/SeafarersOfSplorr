#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.h>
#include <Data.Game.Avatar.Rations.h>
#include <functional>
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Plights.h"
#include "Game.Avatar.Rations.h"
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
	std::optional<game::Item> Rations::Read()
	{
		auto itemId = data::game::avatar::Rations::Read(Player::GetAvatarId());
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Rations::Write(const std::optional<game::Item>& item)
	{
		data::game::avatar::Rations::Clear(Player::GetAvatarId());
		if (item)
		{
			data::game::avatar::Rations::Write(Player::GetAvatarId(), (int)item.value());
		}
	}
}