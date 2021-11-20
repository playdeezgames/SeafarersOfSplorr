#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Avatar 
	{
		int state;
		std::string name;

		static void Write(int, const Avatar&);
		static std::optional<Avatar> Read(int);

		static int NextId();
		static std::list<int> All();
	};
}
