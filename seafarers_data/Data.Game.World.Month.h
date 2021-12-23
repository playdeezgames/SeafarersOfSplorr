#pragma once
#include <string>
#include <list>
#include <optional>
namespace data::game::world
{
	struct Month
	{
		int ordinal;
		std::string name;
		int days;

		static void Initialize();

		static std::optional<Month> Read(int, int);//eliminate
		static std::list<Month> All(int);//change to return list of int

		static std::list<int> AllOrdinals(int);
		static void Write(int, int, const std::string&, int);
		static std::optional<std::string> ReadName(int, int);
		static std::optional<int> ReadDays(int, int);
		static int YearLength(int);
		static void Clear(int);
	};
}