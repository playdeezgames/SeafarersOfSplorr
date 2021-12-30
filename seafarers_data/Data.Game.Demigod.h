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

		typedef int demigodid_t;
		static void Initialize();
		static void Clear();

		static std::list<demigodid_t> All();
		static std::optional<Demigod> Read(int);
		static int Write(const Demigod&);
	};
}
