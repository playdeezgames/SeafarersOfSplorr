#include "State.InPlay.Globals.h"
namespace state::in_play
{
	void PlayMainTheme()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	static int crewDetailCharacterId = 0;

	void SetCrewDetailCharacterId(int characterId)
	{
		crewDetailCharacterId = characterId;
	}

	int GetCrewDetailCharacterId()
	{
		return crewDetailCharacterId;
	}

	static std::string renameShipAdjective = "";

	void SetRenameShipAdjective(const std::string& adjective)
	{
		renameShipAdjective = adjective;
	}

	const std::string& GetRenameShipAdjective()
	{
		return renameShipAdjective;
	}
}
