#pragma once
namespace state::in_play
{
	struct CrewDetail
	{
		static void SetAvatarId(int);
		static int GetAvatarId();
		static void Start();
	};
}