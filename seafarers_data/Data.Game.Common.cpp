#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.SQLite.Store.h"
#include "Data.SQLite.Stores.h"
namespace data::game
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";

	std::list<Common::Record> Common::Execute(const std::string& query)
	{
		return data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
	}

	std::optional<Common::Record> Common::TryExecuteForOne(const std::string& query)
	{
		auto records = Execute(query);
		if (!records.empty())
		{
			return records.front();
		}
		return std::nullopt;
	}

	std::function<std::list<Common::Record>()> Common::DoExecute(const std::string& query)
	{
		return [query]() 
		{
			return Execute(query);
		};
	}


	std::function<void()> Common::Run(const std::string& query)
	{
		return [query]()
		{
			Execute(query);
		};
	}
	common::XY<double> Common::ToXY(const Common::Record& record)
	{
		return
		{
			common::Data::ToDouble(record.find(FIELD_X)->second),
			common::Data::ToDouble(record.find(FIELD_Y)->second)
		};
	}

	int Common::LastInsertedIndex()
	{
		return (int)data::sqlite::Stores::LastInsertedIndex(data::sqlite::Store::IN_MEMORY);
	}

	double Common::ToDouble(const Common::Record& record, const std::string& columnName)
	{
		return common::Data::ToDouble(record.find(columnName)->second);
	}

	int Common::ToInt(const Common::Record& record, const std::string& columnName)
	{
		return common::Data::ToInt(record.find(columnName)->second);
	}

	std::string Common::ToString(const Common::Record& record, const std::string& columnName)
	{
		return record.find(columnName)->second;
	}
}