#pragma once
#include "Game.Characters.Counter.h"
#include <optional>
#include <string>
namespace game::characters
{
	namespace counters
	{
		struct Hunger
		{
			static std::optional<int> Change(int, int);
			static void Reset(int);
		};
		struct Starvation
		{
			static std::optional<int> Change(int, int);
			static void Reset(int);
		};
		struct Wounds
		{
			static std::optional<int> Change(int, int);
		};
	}
	struct Counters
	{
		static const std::string& GetName(const Counter&);
		static void Write(int, const Counter&, int);
		static std::optional<int> Read(int, const Counter&);
		static void Initialize(int);
	};
}
