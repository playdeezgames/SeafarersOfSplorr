#pragma once
#include <optional>
#include <string>
namespace data::game::avatar//20211011
{
	struct DemigodFavor
	{
		static void Write(int, const std::string&, double);
		static std::optional<double> Read(int, const std::string&);
		static void Clear(int);
	};
}
