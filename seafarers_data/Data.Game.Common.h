#pragma once
#include <functional>
#include <format>
#include <list>
#include <map>
#include <string>
namespace data::game
{
	struct Common
	{
		static std::list<std::map<std::string, std::string>> Execute(const std::string&);
		template<typename ...Ts>
		static std::list<std::map<std::string, std::string>> Execute(const std::string& query, Ts... args)
		{
			return Common::Execute(std::format(query, args...));
		}
		static std::function<void()> Run(const std::string&);
	};

}
