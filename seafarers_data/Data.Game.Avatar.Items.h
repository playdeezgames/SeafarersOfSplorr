#pragma once
#include <map>
namespace data::game::avatar::Items
{
	size_t Read(const int&);
	std::map<int, size_t> ReadAll();
	void Write(const int&, const size_t&);
	void Clear();
}
