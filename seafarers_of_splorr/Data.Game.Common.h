#pragma once
#include <map>
#include <list>
#include <string>
#include <optional>
namespace data::game::Common
{
	std::list<std::map<std::string, std::string>> Execute(const std::string&);
	std::string QuoteString(const std::string&);//TODO: does this belong in common data isntead?
	std::string OfOptionalInt(const std::optional<int>&);//TODO: does this belong in common data isntead?
	const int AVATAR_ID = 1;
}
