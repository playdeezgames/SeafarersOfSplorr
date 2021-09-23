#pragma once
namespace data::game
{
	struct FishGame
	{
		static void Clear();
		static int ReadGuesses();
		static bool ReadGivenUp();
		static void WriteGuesses(int);
		static void WriteGivenUp(bool);
		static void Start(int);
	};
}
