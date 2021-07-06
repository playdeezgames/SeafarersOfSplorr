#include "Common.Utility.h"
#include <sstream> //we need this one here
#include <filesystem>
#include <io.h>
namespace common::Utility
{
	std::vector<std::string> ParseCommandLine(int argc, char** argv)
	{
		std::vector<std::string> arguments;
		arguments.reserve(argc);
		for (int index = 0; index < argc; ++index)
		{
			arguments.push_back(std::string(argv[index]));
		}
		return arguments;
	}

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

	bool FileExists(const std::string& filename)
	{
		return std::filesystem::exists(filename);
	}

	unsigned char GetFileCheckSum(const std::string& filename)
	{
		unsigned char result = 0;
		FILE* file = nullptr;
		fopen_s(&file, filename.c_str(), "rb");
		unsigned char data;
		if (file)
		{
			while (!feof(file))
			{
				fread(&data, 1, 1, file);
				result += data;
			}
			fclose(file);
		}
		return result;
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

	const double TAU = 2.0 * 3.141592653589793;

	double ToDegrees(double radians)
	{
		return ModuloDouble(radians * DEGREES / TAU, DEGREES);
	}

	double ToRadians(double degrees)
	{
		return degrees * TAU / DEGREES;
	}
}

