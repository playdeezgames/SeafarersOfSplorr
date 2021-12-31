#pragma once
#include <optional>
namespace data::game::character
{
	struct Counter
	{
		Counter() = delete;
		static void Initialize();
		static void Write(int characterId, int counter, int value);
		static std::optional<int> Read(int characterId, int counter);
		static void Clear(int characterId, int counter);
	};
}
