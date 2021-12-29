#pragma once
#include <optional>
#include <map>
namespace data::game::character
{
	struct Characteristic
	{
		typedef int characterid_t;
		typedef int characteristic_t;
		typedef int value_t;
		static void Initialize();
		static void Write(characterid_t, characteristic_t, value_t);
		static std::optional<value_t> Read(characterid_t, characteristic_t);
		static std::map<characteristic_t, value_t> Read(characterid_t);
	};
}
