#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.SQLite.Store.h"
#include "Data.SQLite.Stores.h"
#include <sstream>
namespace data::game//20211011
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";

	std::list<std::map<std::string, std::string>> Common::Execute(const std::string& query)
	{
		return data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
	}

	std::function<void()> Common::Run(const std::string& query)
	{
		return [query]()
		{
			Execute(query);
		};
	}
	common::XY<double> Common::ToXY(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToDouble(record.find(FIELD_X)->second),
			common::Data::ToDouble(record.find(FIELD_Y)->second)
		};

	}

}