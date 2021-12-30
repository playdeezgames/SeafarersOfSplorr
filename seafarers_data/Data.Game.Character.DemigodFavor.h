#pragma once
#include <optional>
namespace data::game::character
{
	struct DemigodFavor
	{
		typedef int characterid_t;
		typedef int demigodid_t;
		typedef double favor_t;
		static void Initialize();
		static void Write(characterid_t, demigodid_t, favor_t);
		static std::optional<favor_t> Read(characterid_t, demigodid_t);
		static void ClearAll();
	};
}
