#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game//20211015
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
		size_t offeringCooldown;

		static void Clear();
		static std::list<Demigod> All();
		static std::optional<Demigod> Read(const std::string&);
		static int Write(const Demigod&);
	};
}
