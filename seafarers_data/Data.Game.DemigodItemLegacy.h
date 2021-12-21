#pragma once
#include <optional>
namespace data::game
{
	struct DemigodItemLegacy
	{
		static std::optional<double> Read(int, int);
		static void Write(int, int, double);
		static void Clear();
	};
}
