#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Character 
	{
		int state;
		std::string name;

		static int Create(const Character&);
		static void Write(int, const Character&);
		static std::optional<Character> Read(int);

		static std::list<int> All();
	};
}
