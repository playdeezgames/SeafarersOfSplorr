#pragma once
#include <Common.XY.h>
#include <functional>
#include <format>
#include <vector>
#include <map>
#include <optional>
#include <string>
namespace data::game
{
	struct Common
	{
		typedef std::map<std::string, std::string> Record;
		static std::vector<Record> Execute(const std::string_view&);
		static std::optional<Record> TryExecuteForOne(const std::string_view&);
		template<typename ...Ts>
		static std::vector<Record> Execute(const std::string_view& query, Ts... args)
		{
			return Common::Execute(std::format(query, args...));
		}
		template<typename ...Ts>
		static std::optional<Record> TryExecuteForOne(const std::string_view& query, Ts... args)
		{
			return Common::TryExecuteForOne(std::format(query, args...));
		}

		static common::XY<double> ToXY(const Record&);//TODO: eliminate?
		static int LastInsertedIndex();

		static std::optional<double> TryToDouble(const std::optional<Record>& record, const std::string_view& columnName);
		static double ToDouble(const Record& record, const std::string_view& columnName);

		static std::optional<int> TryToInt(const std::optional<Record>& record, const std::string_view& columnName);
		static int ToInt(const Record& record, const std::string_view& columnName);
		static std::function<int(const Record&)> DoToInt(const std::string_view& columnName);

		static std::optional<std::string> TryToString(const std::optional<Record>& record, const std::string_view& columnName);
		static std::string ToString(const Common::Record& record, const std::string_view& columnName);
		static std::function<std::string(const Record&)> DoToString(const std::string_view& columnName);
	};

}
