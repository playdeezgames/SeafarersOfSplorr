#pragma once
#include <vector>
#include <optional>
#include <string>
namespace data::game
{
	struct Tribe
	{
		static void Initialize();
		static int Create(const std::string& name);
		static std::vector<int> All();
		static std::optional<std::string> ReadName(int tribeId);
		static void Clear();
	};
}
