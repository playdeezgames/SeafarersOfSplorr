#pragma once
#include <list>
#include <string>
namespace data::game
{
	struct Demigod
	{
		std::string name;
		size_t patronWeight;
		static void Clear();
		static void Add(const Demigod&);
		static std::list<Demigod> All();
	};
}
