#pragma once
#include <list>
namespace game::session
{
	struct Fisheries
	{
		Fisheries(const std::list<int>&);
		bool HasAny() const;
	private:
		std::list<int> fisheryIds;
	};
}
