#pragma once
#include <optional>
namespace data::game
{
	struct DemigodItem//TODO: becomes item category
	{
		static std::optional<double> Read(int, int);
		static void Write(int, int, double);
		static void Clear();
	};
}
