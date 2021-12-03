#pragma once
#include <optional>
#include <string>
namespace game::islands
{
	struct Taverns
	{
		static std::optional<std::string> GetName(int);
	};
}
