#pragma once
#include <list>
#include <string>
#include <tuple>
namespace data::game::avatar
{
	struct Log
	{
		static void Clear(int);
		static void Write(int,const std::string&, const std::string&);
		static std::list<std::tuple<std::string, std::string>> Read(int,size_t);
		static void ClearAll();
	};
}
