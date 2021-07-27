#include "Common.Data.h"
#include <filesystem>
#include <io.h>
#include <sstream> //we need this one here
namespace common::Data
{
	int StringToInt(const std::string& text)
	{
		std::stringstream ss;
		ss.str(text);
		int result = 0;
		ss >> result;
		return result;
	}

	std::optional<int> StringToOptionalInt(const std::string& text)
	{
		if (!text.empty())
		{
			return StringToInt(text);
		}
		return std::nullopt;
	}


	double StringToDouble(const std::string& text)
	{
		std::stringstream ss;
		ss.str(text);
		double result = 0;
		ss >> result;
		return result;
	}


	int ToPercentage(int value, int maximum)
	{
		return value * 100 / maximum;
	}

	double ClampDouble(double value, double minimum, double maximum)
	{
		return
			(value < minimum) ? (minimum) :
			(value > maximum) ? (maximum) :
			value;
	}

	double ModuloDouble(double value, double divisor)
	{
		return value - floor(value / divisor) * divisor;
	}
}