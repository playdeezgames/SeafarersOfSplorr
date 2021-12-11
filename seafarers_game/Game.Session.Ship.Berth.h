#pragma once
namespace game::session::ship
{
	struct Berth
	{
		Berth(int, int);
	private:
		int shipId;
		int characterId;
	};
}
