#pragma once
#include <Common.XY.h>
#include "Game.Avatar.AtSeaState.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar::AtSea
{
	void Reset(const game::Difficulty&);

	double GetHeading();
	void SetHeading(double);

	double GetSpeed();
	void SetSpeed(double);

	common::XY<double> GetLocation();

	enum class MoveResult
	{
		MOVED,
		CLAMPED
	};
	MoveResult Move();
	std::optional<game::avatar::AtSeaState> GetState();
}
