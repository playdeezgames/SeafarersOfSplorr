#include <Data.Game.Island.h>
#include "Game.Session.Island.h"
#include "Game.Session.Island.Markets.h"
#include "Game.Session.Island.Districts.h"
namespace game::session
{
	static std::optional<common::XY<double>> TryGetLocation(int islandId)
	{
		return data::game::Island::ReadLocation(islandId);
	}

	common::XY<double> Island::GetLocation() const
	{
		return TryGetLocation(islandId).value();
	}

	std::string Island::GetName() const
	{
		return data::game::Island::ReadName(islandId).value();
	}

	int Island::GetPatronDemigodId() const
	{
		return data::game::Island::ReadPatronDemigodId(islandId).value();
	}

	void Island::Populate(const game::Difficulty& difficulty) const
	{
		game::session::island::Markets(islandId).Populate(difficulty);
		game::session::island::Districts(islandId).Populate(difficulty);
	}

	void Island::ApplyTurnEffects() const
	{
		game::session::island::Markets(islandId).ApplyTurnEffects();
		game::session::island::Districts(islandId).ApplyTurnEffects();
	}

	double Island::DistanceFrom(const common::XY<double>& location) const
	{
		return (GetLocation() - location).GetMagnitude();
	}

	double Island::DistanceTo(const Island& otherIsland) const
	{
		return DistanceFrom(otherIsland.GetLocation());
	}

}