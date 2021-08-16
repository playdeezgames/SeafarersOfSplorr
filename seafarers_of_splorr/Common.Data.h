#pragma once
#include <string>
#include <optional>
namespace common::Data
{
	int ToInt(const std::string&);
	std::optional<int> ToOptionalInt(const std::string&);
	std::optional<double> StringToOptionalDouble(const std::string&);
	double StringToDouble(const std::string&);
	int ToPercentage(int, int);
	double ClampDouble(double, double, double);
	double ModuloDouble(double, double);
}