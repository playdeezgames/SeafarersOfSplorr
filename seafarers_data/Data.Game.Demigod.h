#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Demigod
	{
		int id;
		std::string name;
		size_t patronWeight;
		double blessingThreshold;
		double blessingMultiplier;
		int blessingPlightId;
		double curseThreshold;
		double curseMultiplier;
		int cursePlightId;

		static void Initialize();
		static void Clear();
		static std::list<Demigod> All();
		static std::optional<Demigod> Read(int);
		static int Write(const Demigod&);
	};
}
