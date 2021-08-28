#pragma once
#include <list>
#include <string>
namespace data::game
{
	struct Demigod
	{
		std::string name;
		size_t patronWeight;
		//favor threshold
		//favor threshold multiplier
		//disfavor threshold
		//disfavor multiplier
		static void Clear();
		static void Add(const Demigod&);
		static std::list<Demigod> All();
	};
}
