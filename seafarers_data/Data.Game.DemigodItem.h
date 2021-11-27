#pragma once
#include <optional>
#include <string>
namespace data::game//20211014
{
	struct DemigodItem
	{
		static std::optional<double> Read(int, int);
		static void Write(int, int, double);
		static void Clear();
	};
}
