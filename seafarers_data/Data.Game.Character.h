#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Character 
	{
		int state;
		std::string name;

		static void Write(int, const Character&);
		static std::optional<Character> Read(int);

		static int NextId();
		static std::list<int> All();
	};
}
