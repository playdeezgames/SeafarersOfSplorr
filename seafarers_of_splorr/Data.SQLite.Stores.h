#pragma once
#include <string>
#include <map>
#include <list>
#include "Data.SQLite.Store.h"
namespace data::sqlite::Stores
{
	std::list<std::map<std::string, std::string>> Execute(const data::sqlite::Store&, const std::string&);
	void Copy(const data::sqlite::Store&, const data::sqlite::Store&);
	void Bounce(const data::sqlite::Store&);
}
