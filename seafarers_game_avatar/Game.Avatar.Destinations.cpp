#include <Data.Game.Character.Destination.h>
#include <Data.Game.Character.DestinationName.h>
#include "Game.Avatar.Destinations.h"
#include <Game.Player.h>
namespace game::avatar
{
	std::optional<common::XY<double>> Destinations::ReadLocation(const Destination& index)
	{
		return data::game::character::Destination::Read(Player::GetAvatarId(), (int)index);
	}

	void Destinations::WriteLocation(const Destination& index, const std::optional<common::XY<double>>& destination)
	{
		data::game::character::Destination::Clear(Player::GetAvatarId(), (int)index);
		if (destination)
		{
			data::game::character::Destination::Write(Player::GetAvatarId(), (int)index, destination.value());
		}
	}

	static const std::list<Destination> allDestinations = 
	{
		Destination::ONE,
		Destination::TWO,
		Destination::THREE,
		Destination::FOUR
	};

	const std::list<Destination>& Destinations::All()
	{
		return allDestinations;
	}

	std::optional<std::string> Destinations::ReadName(const Destination& destination)
	{
		return data::game::character::DestinationName::Read(Player::GetAvatarId(), (int)destination);
	}

	void Destinations::WriteName(const Destination& destination, const std::string& name)
	{
		data::game::character::DestinationName::Write(Player::GetAvatarId(), (int)destination, name);
	}
}