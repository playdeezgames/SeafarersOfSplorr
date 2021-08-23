#pragma once
#include <list>
#include <string>
#include <tuple>
namespace data::game::avatar
{
	class Log
	{
	public:
		static void Clear();
		static void Write(const std::string&, const std::string&);
		static std::list<std::tuple<std::string, std::string>> Read(size_t);
	};
}
