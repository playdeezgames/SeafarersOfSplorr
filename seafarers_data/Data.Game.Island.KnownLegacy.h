#pragma once
#include <list>
#include <optional>
namespace data::game::island
{
	struct KnownLegacy
	{
		static void Write(int);
		static bool Read(int);
		static void Clear();
		static std::list<int> All();
	};
}