#pragma once
#include <string>
#include <optional>
namespace common::Data
{
	double ToDouble(const std::string&);
	int ToInt(const std::string&);

	std::optional<int> ToOptionalInt(const std::string&);
	std::optional<double> ToOptionalDouble(const std::string&);

	int ToPercentage(int, int);

	double ClampDouble(double, double, double);
	double ModuloDouble(double, double);
}