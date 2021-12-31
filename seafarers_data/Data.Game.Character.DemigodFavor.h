#pragma once
#include <optional>
namespace data::game::character
{
	struct DemigodFavor
	{
		static void Initialize();
		static void Write(int characterId, int demigodId, double favor);
		static std::optional<double> Read(int characterId, int demigodId);
		static void ClearAll();
	};
}
