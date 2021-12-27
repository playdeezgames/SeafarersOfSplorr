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

		static void Initialize();
		static void Write(int, int, double, int);
		static std::optional<double> ReadFavor(int, int);
		static std::optional<int> ReadOfferingCooldown(int, int);
		static void WriteOfferingCooldown(int, int, int);
		static void ClearAll();

		static std::list<Cooldown> ReadOfferingCooldowns();
		static void WriteOfferingCooldowns(const std::list<Cooldown>&);
	};
}
