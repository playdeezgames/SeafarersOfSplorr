#pragma once
#include <vector>
#include <optional>
#include <string>
namespace data::game
{
	struct Demigod
	{
		static void Initialize();
		static void Clear();
		static std::vector<int> All();
		static int Create(const std::string& name, size_t patronWeight);
		static std::optional<std::string> ReadName(int demigodId);
		static std::optional<size_t> ReadPatronWeight(int demigodId);
	};
}
