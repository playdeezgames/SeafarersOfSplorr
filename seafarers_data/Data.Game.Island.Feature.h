#pragma once
namespace data::game::island
{
	struct Feature
	{
		static void Clear();
		static void Write(int, int);
		static void Clear(int, int);
		static bool Read(int, int);
	};
}
