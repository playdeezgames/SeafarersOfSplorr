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

		static int Create(const Character&);//make a Create(int, string)
		static void Write(int, const Character&);//make a Write(int, int) and Write(int, string)
		static std::optional<Character> Read(int);//make a ReadState(int) and ReadName(int)

		static int Create(int, const std::string&);
		static void WriteState(int, int);
		static std::optional<int> ReadState(int);
		static std::optional<std::string> ReadName(int);
		static void Initialize();
		static std::list<int> All();
	};
}
