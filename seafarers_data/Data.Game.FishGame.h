#pragma once
#include <optional>
namespace data::game
{
	struct FishGame
	{
		static void Clear();
		static int ReadGuesses();
		static bool ReadGivenUp();
		static std::optional<int> ReadFisheryId();
		static void WriteGuesses(int);
		static void WriteGivenUp(bool);
		static void WriteFisheryId(std::optional<int>);
		static void Start(int);
	};
}
