#pragma once
#include <optional>
#include <map>
namespace data::game::character
{
	struct Characteristic
	{
		Characteristic() = delete;
		static void Initialize();
		static void Write(int characterId, int characteristic, int value);
		static std::optional<int> Read(int characterId, int characteristic);
		static std::map<int, int> Read(int characterId);
	};
}
