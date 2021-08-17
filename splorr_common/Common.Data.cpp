#include "Common.Data.h"
namespace common::Data
{
	int ToInt(const std::string& text)
	{
		std::stringstream ss;
		ss.str(text);
		int result = 0;
		ss >> result;
		return result;
	}

	std::optional<int> ToOptionalInt(const std::string& text)
	{
		if (!text.empty())
		{
			return ToInt(text);
		}
		return std::nullopt;
	}


	double ToDouble(const std::string& text)
	{
		std::stringstream ss;
		ss.str(text);
		double result = 0;
		ss >> result;
		return result;
	}

	std::optional<double> ToOptionalDouble(const std::string& text)
	{
		if (!text.empty())
		{
			return ToDouble(text);
		}
		return std::nullopt;
	}

	std::optional<int> ToPercentage(int value, int maximum)
	{
		if (maximum != 0)
		{
			return value * 100 / maximum;
		}
		return std::nullopt;
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

	std::string QuoteString(const std::string& original)
	{
		std::stringstream ss;
		ss << "'";
		for (auto ch : original)
		{
			if (ch == '\'')
			{
				ss << "''";
			}
			else
			{
				ss << ch;
			}
		}
		ss << "'";
		return ss.str();
	}
}