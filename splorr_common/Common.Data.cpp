#include <algorithm>
#include "Common.Data.h"
namespace common
{
	int Data::ToInt(const std::string& text)
	{
		return std::stoi(text);
	}

	std::optional<int> Data::ToOptionalInt(const std::string& text)
	{
		if (!text.empty())
		{
			return ToInt(text);
		}
		return std::nullopt;
	}


	double Data::ToDouble(const std::string& text)
	{
		return std::stod(text);
	}

	std::optional<double> Data::ToOptionalDouble(const std::string& text)
	{
		if (!text.empty())
		{
			return ToDouble(text);
		}
		return std::nullopt;
	}

	std::optional<int> Data::ToPercentage(int value, int maximum)
	{
		if (maximum != 0)
		{
			return value * 100 / maximum;
		}
		return std::nullopt;
	}

	std::optional<double> Data::ToPercentage(double value, double maximum)
	{
		if (maximum != 0.0)
		{
			return value * 100.0 / maximum;
		}
		return std::nullopt;
	}

	double Data::ClampDouble(double value, double minimum, double maximum)
	{
		return
			(value < minimum) ? (minimum) :
			(value > maximum) ? (maximum) :
			value;
	}

	std::optional<double> Data::ModuloDouble(double value, double divisor)
	{
		if (divisor != 0.0)
		{
			return value - floor(value / divisor) * divisor;
		}
		return std::nullopt;
	}

	std::string Data::QuoteString(const std::string& original)
	{
		std::string result{};
		result.push_back('\'');
		std::for_each(
			original.begin(), 
			original.end(), 
			[&result](const auto& character) 
			{
				if (character == '\'')
				{
					result.push_back(character);
				}
				result.push_back(character);
			});
		result.push_back('\'');
		return result;
	}
}