#pragma once
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <sstream>
#include <string>
namespace data::game::Common
{
	std::list<std::map<std::string, std::string>> Execute(const std::string&);
	std::function<void()> Run(const std::string&);

	const int AVATAR_ID = 1;
}
