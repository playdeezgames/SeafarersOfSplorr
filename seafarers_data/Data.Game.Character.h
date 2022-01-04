#pragma once
#include <vector>
#include <optional>
#include <string>
namespace data::game
{
	struct Character 
	{
		Character() = delete;
		static int Create(int state, const std::string& name, int originIslandId, int tribeId);
		static void WriteState(int characterId, int state);
		static std::optional<int> ReadState(int characterId);
		static std::optional<std::string> ReadName(int characterId);
		static std::optional<int> ReadOriginIslandId(int characterId);
		static std::optional<int> ReadTribeId(int characterId);
		static void Initialize();
		static std::vector<int> All();
	};
}
