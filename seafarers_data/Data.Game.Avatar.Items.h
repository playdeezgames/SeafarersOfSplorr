#pragma once
#include <map>
namespace data::game::avatar
{
	class Items
	{
	public:
		static size_t Read(const int&);
		static std::map<int, size_t> ReadAll();
		static void Write(const int&, const size_t&);
		static void Clear();
	};
}
