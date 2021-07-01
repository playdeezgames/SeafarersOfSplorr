#pragma once
#include <string>
#include <map>
#include <list>
namespace data::sqlite
{
	enum class Store
	{
		IN_MEMORY
	};
}
namespace data::sqlite::Stores
{
	std::list<std::map<std::string, std::string>> Execute(const data::sqlite::Store&, const std::string&);
}
