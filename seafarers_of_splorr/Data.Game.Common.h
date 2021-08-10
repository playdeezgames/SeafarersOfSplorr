#pragma once
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <string>
namespace data::game::Common
{
	std::list<std::map<std::string, std::string>> Execute(const std::string&);
	std::function<void()> Run(const std::string&);
	std::string QuoteString(const std::string&);//TODO: does this belong in common data instead?
	std::string OfOptionalInt(const std::optional<int>&);//TODO: does this belong in common data instead?
	const int AVATAR_ID = 1;
}
