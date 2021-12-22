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
		static void Write(int, const Month&);
		static std::optional<Month> Read(int, int);
		static std::list<Month> All(int);
		static int YearLength(int);
		static void Clear(int);
	};
}