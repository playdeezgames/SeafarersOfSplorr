#include "Data.Game.Common.h"
#include "Data.SQLite.Stores.h"
namespace data::game::Common
{
	std::list<std::map<std::string, std::string>> Execute(const std::string& query)
	{
		return data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
	}
}