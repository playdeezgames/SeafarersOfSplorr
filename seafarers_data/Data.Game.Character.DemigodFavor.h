#pragma once
#include <optional>
#include <list>
namespace data::game::character
{
	struct DemigodFavor
	{
		struct Cooldown
		{
			int characterId;
			int demigodId;
			int cooldown;
			constexpr Cooldown(int characterId, int demigodId, int cooldown) 
				: characterId(characterId), demigodId(demigodId), cooldown(cooldown) {}
		};

		typedef int characterid_t;
		typedef int demigodid_t;
		typedef double favor_t;
		typedef int cooldown_t;
		static void Initialize();
		static void Write(characterid_t, demigodid_t, favor_t, cooldown_t);
		static void WriteFavor(characterid_t, demigodid_t, favor_t);
		static std::optional<favor_t> ReadFavor(characterid_t, demigodid_t);
		static std::optional<cooldown_t> ReadOfferingCooldown(characterid_t, demigodid_t);
		static void WriteOfferingCooldown(characterid_t, demigodid_t, cooldown_t);
		static void ClearAll();

		static std::list<Cooldown> ReadOfferingCooldowns();
		static void WriteOfferingCooldowns(const std::list<Cooldown>&);
	};
}
