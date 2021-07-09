#pragma once
#include "Common.XY.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::Avatar
{
	common::XY<double> GetLocation();
	double GetHeading();
	void SetHeading(double);
	double GetSpeed();
	void SetSpeed(double);
	void Reset(const game::Difficulty&);
	void Move();
	bool Dock();
	std::optional<common::XY<double>> GetDockedLocation();
	bool Undock();
}
