#pragma once
#include <list>
#include <string>
#include <tuple>
namespace data::game::avatar::Log
{
	void Clear();
	void Write(const std::string&, const std::string&);
	std::list<std::tuple<std::string, std::string>> Read(size_t);
}
