#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Character 
	{
		static int Create(int, const std::string&, int, int);
		static void WriteState(int, int);
		static std::optional<int> ReadState(int);
		static std::optional<std::string> ReadName(int);
		static std::optional<int> ReadOriginIslandId(int);
		static std::optional<int> ReadTribeId(int);
		static void Initialize();
		static std::list<int> All();
	};
}
