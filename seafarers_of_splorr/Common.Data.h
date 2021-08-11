#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <optional>
namespace common::Data
{
	int StringToInt(const std::string&);
	std::optional<int> StringToOptionalInt(const std::string&);
	std::optional<double> StringToOptionalDouble(const std::string&);
	double StringToDouble(const std::string&);
	int ToPercentage(int, int);
	double ClampDouble(double, double, double);
	double ModuloDouble(double, double);
}