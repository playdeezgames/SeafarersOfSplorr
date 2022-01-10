#pragma once
namespace state::scratch_pad
{
	struct CrewDetail
	{
		CrewDetail() = delete;
		static void SetCharacterId(int characterId);
		static int GetCharacterId();
	private:
		static int crewDetailCharacterId;
	};
}
