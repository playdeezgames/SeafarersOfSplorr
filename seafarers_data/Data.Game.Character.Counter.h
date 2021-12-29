#pragma once
#include <optional>
namespace data::game::character
{
	struct Counter
	{
		typedef int characterid_t;
		typedef int counter_t;
		typedef int value_t;
		static void Initialize();
		static void Write(characterid_t, counter_t, value_t);
		static std::optional<value_t> Read(characterid_t, counter_t);
		static void Clear(characterid_t, counter_t);
	};
}
