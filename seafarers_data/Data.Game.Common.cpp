#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.SQLite.Store.h"
#include "Data.SQLite.Stores.h"
namespace data::game
{
	static const std::string_view FIELD_X = "X";
	static const std::string_view FIELD_Y = "Y";

	std::vector<Common::Record> Common::Execute(const std::string_view& query)
	{
		return data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, std::string(query));
	}

	std::optional<Common::Record> Common::TryExecuteForOne(const std::string_view& query)
	{
		auto records = Execute(query);
		if (!records.empty())
		{
			return records.front();
		}
		return std::nullopt;
	}

	common::XY<double> Common::ToXY(const Common::Record& record)
	{
		return
		{
			common::Data::ToDouble(record.find(std::string(FIELD_X))->second),
			common::Data::ToDouble(record.find(std::string(FIELD_Y))->second)
		};
	}

	int Common::LastInsertedIndex()
	{
		return (int)data::sqlite::Stores::LastInsertedIndex(data::sqlite::Store::IN_MEMORY);
	}

	std::optional<double> Common::TryToDouble(const std::optional<Record>& record, const std::string_view& columnName)
	{
		if (record)
		{
			return ToDouble(record.value(), columnName);
		}
		return std::nullopt;
	}

	double Common::ToDouble(const Common::Record& record, const std::string_view& columnName)
	{
		return common::Data::ToDouble(record.find(std::string(columnName))->second);
	}

	std::optional<int> Common::TryToInt(const std::optional<Record>& record, const std::string_view& columnName)
	{
		if (record)
		{
			return ToInt(record.value(), columnName);
		}
		return std::nullopt;
	}

	int Common::ToInt(const Common::Record& record, const std::string_view& columnName)
	{
		return common::Data::ToInt(record.find(std::string(columnName))->second);
	}

	std::function<int(const Common::Record&)> Common::DoToInt(const std::string_view& columnName)
	{
		return [columnName](const Common::Record& record)
		{
			return ToInt(record, columnName);
		};
	}

	std::optional<std::string> Common::TryToString(const std::optional<Record>& record, const std::string_view& columnName)
	{
		if (record)
		{
			return ToString(record.value(), columnName);
		}
		return std::nullopt;
	}

	std::string Common::ToString(const Common::Record& record, const std::string_view& columnName)
	{
		return record.find(std::string(columnName))->second;
	}

	std::function<std::string(const Common::Record&)> Common::DoToString(const std::string_view& columnName)
	{
		return [columnName](const Common::Record& record)
		{
			return ToString(record, std::string(columnName));
		};
	}
}