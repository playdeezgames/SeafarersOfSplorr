#pragma once
#include <functional>
#include <list>
namespace game::session
{
	struct Fisheries
	{
		Fisheries(std::function<std::list<int>()>);
		bool HasAny() const;
	private:
		std::function<std::list<int>()> fisherySource;
	};
}
