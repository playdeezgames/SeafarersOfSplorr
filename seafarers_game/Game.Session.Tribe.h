#pragma once
#include <string>
namespace game::session
{
	struct Tribe
	{
		constexpr Tribe(int tribeId) : tribeId(tribeId) {}
		std::string GetName() const;
		constexpr static Tribe FromTribeId(int tribeId) { return Tribe(tribeId); }
	private:
		int tribeId;
	};
}