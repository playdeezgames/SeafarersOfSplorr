#include "Data.Game.Common.h"
#include "Data.SQLite.Stores.h"
#include <sstream>
namespace data::game::Common
{
	std::list<std::map<std::string, std::string>> Execute(const std::string& query)
	{
		return data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
	}

	std::string QuoteString(const std::string& original)
	{
		std::stringstream ss;
		ss << "'";
		for (auto ch : original)
		{
			if (ch == '\'')
			{
				ss << "''";
			}
			else
			{
				ss << ch;
			}
		}
		ss << "'";
		return ss.str();
	}

	std::string OfOptionalInt(const std::optional<int>& value)
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
}