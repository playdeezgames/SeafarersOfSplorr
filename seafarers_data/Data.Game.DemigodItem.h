#pragma once
#include <optional>
#include <string>
namespace data::game//20211014
{
	struct DemigodItem
	{
		static std::optional<double> Read(const std::string&, int);
		static void Write(const std::string&, int, double);
		static void Clear();
	};
}
