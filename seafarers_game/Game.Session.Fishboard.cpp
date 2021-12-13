#include "Game.Fishboard.h"
#include "Game.Session.Fishboard.h"
namespace game::session
{
	Fishboard::Fishboard(int characterId)
		: characterId(characterId)
	{

	}

	std::optional<FishboardCell> Fishboard::GetCell(int column, int row) const
	{
		return FishboardCell(characterId, column, row);
	}

	std::optional<double> Fishboard::GetProgressPercentage() const
	{
		return game::Fishboard::ReadProgressPercentage(characterId);
	}
}