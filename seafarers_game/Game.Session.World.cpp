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
	static const int LATEST_INITIAL_YEAR = 1000;
	static const int DAYS_PER_YEAR = 360;

	void World::Populate(const Difficulty& difficulty) const
	{

		auto currencyItemSubtype = CreateWorldCurrencyItemSubtype();

		data::game::World::Write(
			worldId,
			(int)difficulty,
			common::RNG::FromRange(0.0, common::Heading::DEGREES),
			(int)currencyItemSubtype,
			common::RNG::FromRange(EARLIEST_INITIAL_YEAR, LATEST_INITIAL_YEAR) * DAYS_PER_YEAR +
			common::RNG::FromRange(0, DAYS_PER_YEAR));

		GetCharacters().Populate(difficulty);
		GetCalendar().Populate(difficulty);
		GetDemigods().Populate(difficulty);
		GetTribes().Populate(difficulty);
		GetIslands().Populate(difficulty);
	}


	void World::Reset() const
	{
		GetBounds().Reset();
		GetCalendar().Reset();
		GetCharacters().Reset();
		GetCommodities().Reset();
		GetDemigods().Reset();
		GetDistances().Reset();
		GetIslands().Reset();
		GetItemSubtypes().Reset();
		GetTribes().Reset();
		GetWind().Reset();
	}

	void World::ApplyTurnEffects() const
	{
		GetIslands().ApplyTurnEffects();
		GetWind().ApplyTurnEffects();
		GetCalendar().ApplyTurnEffects();
		GetDemigods().ApplyTurnEffects();
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