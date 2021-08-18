#pragma once
#include <functional>
#include <list>
#include <map>
#include <string>
namespace data::sqlite::Stores
{
	void SetConnection(int, const std::string&);
	template<typename TStore>
	std::function<void()> DoSetConnection(const TStore& store, const std::string& connectionString)
	{
		return [store, connectionString]()
		{
			SetConnection((int)store, connectionString);
		};
	}

	std::list<std::map<std::string, std::string>> Execute(int, const std::string&);
	template<typename TStore>
	std::list<std::map<std::string, std::string>> Execute(const TStore& store, const std::string& query)
	{
		return Execute((int)store, query);
	}

	void Copy(int, int);
	template<typename TStore>
	void Copy(const TStore& first, const TStore& second)
	{
		Copy((int)first, (int)second);
	}

	void Bounce(int);
	template<typename TStore>
	void Bounce(const TStore& store)
	{
		Bounce((int)store);
	}
}
