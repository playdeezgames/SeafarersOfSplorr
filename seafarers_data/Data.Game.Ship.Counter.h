#pragma once
#include <optional>
namespace data::game::ship
{
	struct Counter
	{
		Counter() = delete;
		static void Initialize();
		static std::optional<int> Read(int shipId, int counter);
		static void Write(int shipId, int counter, int value);
	};
}
