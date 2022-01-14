#include <Data.Game.Island.h>
#include "Game.Session.Island.h"
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

	Demigod Island::GetPatronDemigod() const
	{
		return Demigod(data::game::Island::ReadPatronDemigodId(islandId).value());
	}

	void Island::Populate(const game::Difficulty& difficulty) const
	{
		GetMarkets().Populate(difficulty);
		GetDistricts().Populate(difficulty);
	}

	void Island::ApplyTurnEffects() const
	{
		GetMarkets().ApplyTurnEffects();
		GetDistricts().ApplyTurnEffects();
	}

	double Island::DistanceTo(const Island& otherIsland) const
	{
		return (GetLocation() - otherIsland.GetLocation()).GetMagnitude();
	}

}