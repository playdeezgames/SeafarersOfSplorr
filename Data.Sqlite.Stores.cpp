#include "Data.SQLite.Stores.h"
#include "sqlite3.h"
#include <memory>
namespace data::sqlite::Stores
{
	static std::map<data::sqlite::Store, std::shared_ptr<sqlite3>> connections;
	const std::map<data::sqlite::Store, std::string> connectionStrings =
	{
		{data::sqlite::Store::IN_MEMORY, ":memory:"}
	};

	static std::shared_ptr<sqlite3> GetConnection(const data::sqlite::Store& store)
	{
		auto iter = connections.find(store);
		if (iter == connections.end())
		{
			sqlite3* db = nullptr;
			sqlite3_open(connectionStrings.find(store)->second.c_str(), &db);
			auto ptr = std::shared_ptr<sqlite3>(db, sqlite3_close);
			connections[store] = ptr;
			return ptr;
		}
		return iter->second;
	}

	static int ExecutionCallback(void* data, int column_count, char** column_values, char** column_names)
	{
		std::list<std::map<std::string, std::string>>* results = (std::list<std::map<std::string, std::string>>*)data;
		std::map<std::string, std::string> record;
		for (int column = 0; column < column_count; ++column)
		{
			record[column_names[column]] = column_values[column];
		}
		results->push_back(record);
		return 0;
	}

	static std::list<std::map<std::string, std::string>> DoExecute(const std::shared_ptr<sqlite3>& connection, const std::string& query)
	{
		std::list<std::map<std::string, std::string>> results;
		sqlite3_exec(connection.get(), query.c_str(), ExecutionCallback, &results, nullptr);
		return results;
	}

	std::list<std::map<std::string, std::string>> Execute(const data::sqlite::Store& store, const std::string& query)
	{
		auto connection = GetConnection(store);
		if (connection)
		{
			return DoExecute(connection, query);
		}
		return std::list<std::map<std::string, std::string>>();
	}
}