#include <Data.Game.Avatar.Destination.h>
#include <Data.Game.Avatar.DestinationName.h>
#include "Game.Avatar.Destinations.h"
#include <Game.Player.h>
namespace game::avatar//20211017
{
	std::optional<common::XY<double>> Destinations::ReadLocation(const Destination& index)
	{
		return data::game::avatar::Destination::Read(Player::GetAvatarId(), (int)index);
	}

	void Destinations::WriteLocation(const Destination& index, const std::optional<common::XY<double>>& destination)
	{
		data::game::avatar::Destination::Clear(Player::GetAvatarId(), (int)index);
		if (destination)
		{
			data::game::avatar::Destination::Write(Player::GetAvatarId(), (int)index, destination.value());
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
		return data::game::avatar::DestinationName::Read(Player::GetAvatarId(), (int)destination);
	}

	void Destinations::WriteName(const Destination& destination, const std::string& name)
	{
		data::game::avatar::DestinationName::Write(Player::GetAvatarId(), (int)destination, name);
	}
}