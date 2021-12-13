#include "Game.Fishboard.h"
#include "Game.Session.FishboardCell.h"
namespace game::session
{
	FishboardCell::FishboardCell(int characterId, int column, int row)
		: characterId(characterId)
		, column(column)
		, row(row)
	{

	}

	std::optional<bool> FishboardCell::IsRevealed() const
	{
		auto cell = game::Fishboard::Read(characterId, { column, row });
		return cell.revealed;
	}

	std::optional<game::Fish> FishboardCell::GetFish() const
	{
		auto cell = game::Fishboard::Read(characterId, { column, row });
		return cell.fish;
	}

	void FishboardCell::Reveal() const
	{
		game::Fishboard::Reveal(characterId, { column, row });
	}
}