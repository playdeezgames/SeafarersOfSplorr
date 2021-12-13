#include "Game.Fishboard.h"
#include "Game.Session.Fishboard.h"
namespace game::session
{
	Fishboard::Fishboard(int characterId)
		: characterId(characterId)
	{

	}

	static std::optional<FishboardCell> TryGetCell(int characterId, int column, int row)
	{
		return FishboardCell(characterId, column, row);
	}

	static std::optional<double> TryGetProgressPercentage(int characterId)
	{
		return game::Fishboard::ReadProgressPercentage(characterId);
	}

	FishboardCell Fishboard::GetCell(int column, int row) const
	{
		return TryGetCell(characterId, column, row).value();
	}

	double Fishboard::GetProgressPercentage() const
	{
		return TryGetProgressPercentage(characterId).value();
	}
}