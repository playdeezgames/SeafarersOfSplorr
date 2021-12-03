#pragma once
#include <optional>
#include <string>
namespace data::game::island
{
	struct Tavern
	{
		int islandId;
		std::string name;

		static std::optional<Tavern> Read(int);
		static void Write(const Tavern&);
	};
}
