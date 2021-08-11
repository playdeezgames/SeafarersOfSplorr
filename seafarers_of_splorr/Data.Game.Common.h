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
	std::string QuoteString(const std::string&);
	std::string OfOptionalInt(const std::optional<int>&);

	template<typename TValue>
	std::string OfOptional(const std::optional<TValue>& value)
	{
		std::stringstream ss;
		if (value.has_value())
		{
			ss << value.value();
		}
		else
		{
			ss << "NULL";
		}
		return ss.str();
	}

	const int AVATAR_ID = 1;
}
