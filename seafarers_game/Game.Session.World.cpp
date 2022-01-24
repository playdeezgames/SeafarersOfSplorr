#include "Game.Session.World.Calendar.h"
#include "Game.Session.World.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.Demigods.h"
#include "Game.Session.Item.Types.h"
#include "Game.Session.Tribes.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.Islands.h"
#include "Game.Session.Skills.h"
#include "Game.Session.World.Wind.h"
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.Session.Item.Type.Commodities.h"
namespace game::session
{
	static item::Type CreateWorldCurrencyItemSubtype()//TODO: should this go to Items::Populate?
	{
		const double WORLD_CURRENCY_JOOLS_AMOUNT = 0.001;
		const std::string WORLD_CURRENCY_JOOLS_NAME = "jools";
		auto result = item::Types()
			.Create(game::item::Category::CURRENCY, WORLD_CURRENCY_JOOLS_NAME);
		game::session::item::type::Commodities(result.operator int())
			.GetCommodity(game::Commodity::JOOLS)
			.SetAmount(WORLD_CURRENCY_JOOLS_AMOUNT);
		return result;
	}

	static const int EARLIEST_INITIAL_YEAR = 500;
	static const int LATEST_INITIAL_YEAR = 1000;
	static const int DAYS_PER_YEAR = 360;

	void World::Populate(const Difficulty& difficulty)
	{
		auto currencyItemSubtype = CreateWorldCurrencyItemSubtype();

		data::game::World::Write(
			WORLD_ID,
			(int)difficulty,
			common::RNG::FromRange(0.0, common::Heading::DEGREES),
			(int)currencyItemSubtype,
			common::RNG::FromRange(EARLIEST_INITIAL_YEAR, LATEST_INITIAL_YEAR) * DAYS_PER_YEAR +
			common::RNG::FromRange(0, DAYS_PER_YEAR));

		Skills::Populate(difficulty);
		world::Calendar().Populate(difficulty);
		session::Demigods().Populate(difficulty);
		Tribes().Populate(difficulty);
		Islands::Populate(difficulty);
	}


	void World::Reset()
	{
		world::Calendar().Reset();
		session::Commodities().Reset();
		session::Demigods().Reset();
		Islands::Reset();
		item::Types().Reset();
		Skills::Reset();
		Tribes().Reset();
	}

	void World::ApplyTurnEffects()
	{
		Islands::ApplyTurnEffects();
		world::Wind::ApplyTurnEffects();
		world::Calendar().ApplyTurnEffects();
		session::Demigods().ApplyTurnEffects();
	}

	game::Difficulty World::GetDifficulty()
	{
		return (Difficulty)data::game::World::ReadDifficulty(WORLD_ID).value();
	}

	item::Type World::GetCurrencyItemSubtype()
	{
		return item::Type(data::game::World::ReadCurrencyItemTypeId(WORLD_ID).value());
	}
}