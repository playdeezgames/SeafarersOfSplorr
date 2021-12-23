#pragma once
#include <Common.XY.h>
#include <functional>
#include <format>
#include <list>
#include <map>
#include <optional>
#include <string>
namespace data::game
{
	struct Common
	{
		typedef std::map<std::string, std::string> Record;
		static std::list<Record> Execute(const std::string&);
		static std::optional<Record> TryExecuteForOne(const std::string&);
		static std::function<std::list<Record>()> DoExecute(const std::string&);
		template<typename ...Ts>
		static std::list<Record> Execute(const std::string& query, Ts... args)
		{
			return Common::Execute(std::format(query, args...));
		}
		template<typename ...Ts>
		static std::optional<Record> TryExecuteForOne(const std::string& query, Ts... args)
		{
			return Common::TryExecuteForOne(std::format(query, args...));
		}
		static std::function<void()> Run(const std::string&);
		static common::XY<double> ToXY(const Record&);
		static int LastInsertedIndex();
		static double ToDouble(const Record&, const std::string&);
		static int ToInt(const Record&, const std::string&);
	};

}
