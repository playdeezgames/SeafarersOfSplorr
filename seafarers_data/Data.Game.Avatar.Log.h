#pragma once
#include <list>
#include <string>
#include <tuple>
namespace data::game::avatar//20211011
{
	struct Log
	{
		std::string color;
		std::string text;

		static void Clear(int);
		static void Write(int,const std::string&, const std::string&);
		static std::list<Log> Read(int, size_t);
		static void ClearAll();
	};
}
