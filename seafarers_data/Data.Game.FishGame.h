#pragma once
#include <optional>
namespace data::game
{
	struct FishGame
	{
		static void Clear(int);
		static int ReadGuesses(int);
		static bool ReadGivenUp(int);
		static std::optional<int> ReadFisheryId(int);
		static void WriteGuesses(int, int);
		static void WriteGivenUp(int, bool);
		static void WriteFisheryId(int, std::optional<int>);
		static void Start(int, int);
	};
}
