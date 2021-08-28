#pragma once
#include <list>
#include <string>
namespace data::game
{
	struct Demigod
	{
		std::string name;
		size_t patronWeight;
		double blessingThreshold;
		double blessingMultiplier;
		int blessingPlightId;
		double curseThreshold;
		double curseMultiplier;
		int cursePlightId;
		static void Clear();
		static void Add(const Demigod&);
		static std::list<Demigod> All();
	};
}
