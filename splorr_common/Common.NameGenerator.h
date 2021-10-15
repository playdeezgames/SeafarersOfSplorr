#pragma once
#include <map>
#include <string>
namespace common//20211015
{
	struct NameGenerator
	{
		std::map<size_t, size_t> lengthGenerator;
		std::map<bool, size_t> vowelStartGenerator;
		std::map<std::string, size_t> vowelGenerator;
		std::map<std::string, size_t> consonantGenerator;

		std::string Generate() const;
	};
}