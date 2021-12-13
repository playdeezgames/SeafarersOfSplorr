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

	static std::optional<bool> TryIsRevealed(int characterId, int column, int row)
	{
		auto cell = game::Fishboard::Read(characterId, { column, row });
		return cell.revealed;
	}

	bool FishboardCell::IsRevealed() const
	{
		return TryIsRevealed(characterId, column, row).value();
	}

	static std::optional<game::Fish> TryGetFish(int characterId, int column, int row)
	{
		auto cell = game::Fishboard::Read(characterId, { column, row });
		return cell.fish;
	}

	game::Fish FishboardCell::GetFish() const
	{
		return TryGetFish(characterId, column, row).value();
	}

	void FishboardCell::Reveal() const
	{
		game::Fishboard::Reveal(characterId, { column, row });
	}
}