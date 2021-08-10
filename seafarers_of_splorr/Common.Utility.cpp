#include "Common.Utility.h"
#include <filesystem>
#include <io.h>
#include <sstream>
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
}

