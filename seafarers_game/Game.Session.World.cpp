#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.Session.Item.Types.h"
#include "Game.Session.World.h"
namespace game::session
{
	static item::Type CreateWorldCurrencyItemSubtype()
	{
		const double WORLD_CURRENCY_JOOLS_AMOUNT = 0.001;
		const std::string WORLD_CURRENCY_JOOLS_NAME = "jools";
		auto result = item::Types().Create(game::item::Category::CURRENCY, WORLD_CURRENCY_JOOLS_NAME);
		result.GetCommodities().GetCommodity(game::Commodity::JOOLS).SetAmount(WORLD_CURRENCY_JOOLS_AMOUNT);
		return result;
	}

	static const int EARLIEST_INITIAL_YEAR = 500;
	static const int LATEST_INITIAL_YEAR = 5000;
	static const int DAYS_PER_YEAR = 360;

	void World::Reset(const Difficulty& difficulty) const
	{
		GetIslands().Reset(difficulty);
		GetItemSubtypes().Reset(difficulty);

		auto currencyItemSubtype = CreateWorldCurrencyItemSubtype();

		data::game::World::Write(
			worldId, 
			(int)difficulty,
			common::RNG::FromRange(0.0, common::Heading::DEGREES),
			(int)currencyItemSubtype,
			common::RNG::FromRange(EARLIEST_INITIAL_YEAR, LATEST_INITIAL_YEAR)* DAYS_PER_YEAR +
				common::RNG::FromRange(0, DAYS_PER_YEAR));

		GetCalendar().Reset(difficulty);
	}

	void World::ApplyTurnEffects() const
	{
		GetIslands().ApplyTurnEffects();
		GetWind().ApplyTurnEffects();
		GetCalendar().ApplyTurnEffects();
	}

	game::Difficulty World::GetDifficulty() const
	{
		return (Difficulty)data::game::World::ReadDifficulty(worldId).value();
	}

	item::Type World::GetCurrencyItemSubtype() const
	{
		return item::Type(data::game::World::ReadCurrencyItemTypeId(worldId).value());
	}

}