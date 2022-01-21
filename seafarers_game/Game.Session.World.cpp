#include "Game.Session.World.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.Demigods.h"
#include "Game.Session.Item.Types.h"
#include "Game.Session.Tribes.h"
#include "Game.Session.World.Islands.h"
#include "Game.Session.World.Skills.h"
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
namespace game::session
{
	static item::Type CreateWorldCurrencyItemSubtype()//TODO: should this go to Items::Populate?
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

		world::Skills().Populate(difficulty);
		GetCalendar().Populate(difficulty);
		session::Demigods().Populate(difficulty);
		Tribes().Populate(difficulty);
		world::Islands().Populate(difficulty);
	}


	void World::Reset() const
	{
		GetBounds().Reset();
		GetCalendar().Reset();
		session::Commodities().Reset();
		session::Demigods().Reset();
		GetDistances().Reset();
		world::Islands().Reset();
		item::Types().Reset();
		world::Skills().Reset();
		Tribes().Reset();
		GetWind().Reset();
	}

	void World::ApplyTurnEffects() const
	{
		world::Islands().ApplyTurnEffects();
		GetWind().ApplyTurnEffects();
		GetCalendar().ApplyTurnEffects();
		session::Demigods().ApplyTurnEffects();
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