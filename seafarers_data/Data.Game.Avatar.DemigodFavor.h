#pragma once
#include <optional>
#include <string>
namespace data::game::avatar
{
	struct DemigodFavor
	{
		static void Write(int, const std::string&, double);
		static std::optional<double> Read(int, const std::string&);
		static void Clear(int);
		static void Write(const std::string&, double);
		static std::optional<double> Read(const std::string&);
		static void Clear();
	};
}
