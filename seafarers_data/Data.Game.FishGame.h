#pragma once
namespace data::game
{
	struct FishGame
	{
		static int ReadGuesses();
		static void WriteGuesses(int);
		static void Clear();
		static bool ReadGivenUp();
		static void WriteGivenUp(bool);
	};
}
