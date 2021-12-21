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

		static void Initialize();
		static int Create(const Character&);//make a Create(int, string)
		static void Write(int, const Character&);//make a Write(int, int) and Write(int, string)
		static std::optional<Character> Read(int);//make a ReadState(int) and ReadName(int)

		static std::list<int> All();
	};
}
