#include <Data.Game.World.h>
#include "Game.Session.World.Bounds.h"
namespace game::session::world
{
	common::XY<double> Bounds::GetSize() const
	{
		return data::game::World::Read().value().size;
	}

	bool Bounds::ClampLocation(common::XY<double>& location) const
	{
		bool result = false;
		auto worldSize = GetSize();
		if (location.GetX() < 0.0)
		{
			result = true;
			location = { 0, location.GetY() };
		}
		if (location.GetX() > worldSize.GetX())
		{
			result = true;
			location = { worldSize.GetX(), location.GetY() };
		}
		if (location.GetY() < 0.0)
		{
			result = true;
			location = { location.GetX(), 0.0 };
		}
		if (location.GetY() > worldSize.GetY())
		{
			result = true;
			location = { location.GetX(), worldSize.GetY() };
		}
		return result;
	}
}