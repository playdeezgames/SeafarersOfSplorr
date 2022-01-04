#pragma once
#include <optional>
#include <vector>
namespace data::game::character
{
	struct Plight
	{
		Plight() = delete;
		static void Initialize();
		static std::vector<int> All(int characterId);
		static void Write(int characterId, int plight, int duration);
		static std::optional<int> ReadDuration(int characterId, int plight);
		static void ClearPlight(int characterId, int plight);
		static void Clear(int characterId);
		static void ClearAll();
	};
}
