#pragma once
#include <sstream>
#include <string>
#include <optional>
namespace common::Data
{
	double ToDouble(const std::string&);
	int ToInt(const std::string&);

	std::optional<int> ToOptionalInt(const std::string&);
	std::optional<double> ToOptionalDouble(const std::string&);

	std::optional<int> ToPercentage(int, int);

	double ClampDouble(double, double, double);
	double ModuloDouble(double, double);//TODO: i am next!

	std::string QuoteString(const std::string&);
	template<typename TValue>
	std::string OfOptional(const std::optional<TValue>& value)
	{
		std::stringstream ss;
		if (value.has_value())
		{
			ss << value.value();
		}
		else
		{
			ss << "NULL";
		}
		return ss.str();
	}
}