#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
#include "Visuals.HorizontalAlignment.h"
namespace game
{
	struct MessageDetail
	{
		common::XY<int> position;
		std::string text;
		std::string color;
		visuals::HorizontalAlignment alignment;
	};
}

