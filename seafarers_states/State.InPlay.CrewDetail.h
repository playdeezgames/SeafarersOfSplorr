#pragma once
namespace state::in_play
{
	struct CrewDetail
	{
		static void SetCharacterId(int);
		static int GetCharacterId();
		static void Start();
	};
}