#pragma once
#include <optional>
namespace data::game::ship
{
	struct Island
	{
		static void Initialize();
		static void Write(int, int);
		static bool Read(int, int);
		static void Clear(int);
		static void Clear();
	};
}