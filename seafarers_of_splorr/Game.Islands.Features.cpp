#include "Data.Game.Island.Feature.h"
#include "Game.Islands.Features.h"
#include "Game.Features.h"
namespace game::islands::Features
{
	bool Read(const common::XY<double>& xy, const game::Feature& feature)
	{
		return data::game::island::Feature::Read(xy, (int)feature);
	}

	void Write(const common::XY<double>& xy, const game::Feature& feature, bool value)
	{
		if (value)
		{
			data::game::island::Feature::Write(xy, (int)feature);
		}
		else
		{
			data::game::island::Feature::Clear(xy, (int)feature);
		}
	}

	void Reset(const game::Difficulty&)
	{
		data::game::island::Feature::ClearAll();
	}
}