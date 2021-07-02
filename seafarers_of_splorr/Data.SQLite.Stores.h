#pragma once
#include <string>
#include <map>
#include <list>
namespace data::sqlite
{
	enum class Store
	{
		IN_MEMORY,
		AUTOSAVE,
		SLOT_1,
		SLOT_2,
		SLOT_3,
		SLOT_4,
		SLOT_5
	};
}
namespace data::sqlite::Stores
{
	std::list<std::map<std::string, std::string>> Execute(const data::sqlite::Store&, const std::string&);
	void Copy(const data::sqlite::Store&, const data::sqlite::Store&);
	void Bounce(const data::sqlite::Store&);
}
