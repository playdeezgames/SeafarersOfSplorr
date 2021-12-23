#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.Session.Item.Types.h"
#include "Game.Session.World.h"
namespace game::session
{
	static const int CURRENT_VERSION = 1;

	struct WorldProperties
	{
		double size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
	};

	static const std::map<game::Difficulty, WorldProperties> worldProperties =
	{
		{game::Difficulty::EASY, {100.0, 10.0, 10.0, 1.0}},
		{game::Difficulty::NORMAL, {150.0, 15.0, 10.0, 1.0}},
		{game::Difficulty::HARD, {200.0, 20.0, 10.0, 1.0}},
		{game::Difficulty::HARDCORE, {250.0, 25.0, 10.0, 1.0}}
	};


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
		GetItemSubtypes().Reset(difficulty);
		auto properties = worldProperties.find(difficulty)->second;

		auto currencyItemSubtype = CreateWorldCurrencyItemSubtype();

		data::game::World::Write(
			worldId, 
			CURRENT_VERSION,
			{properties.size, properties.size},
			properties.minimumIslandDistance,
			properties.viewDistance,
			properties.dockDistance,
			common::RNG::FromRange(0.0, common::Heading::DEGREES),
			(int)currencyItemSubtype,
			common::RNG::FromRange(EARLIEST_INITIAL_YEAR, LATEST_INITIAL_YEAR)* DAYS_PER_YEAR +
				common::RNG::FromRange(0, DAYS_PER_YEAR));

		GetCalendar().Reset(difficulty);
	}

	void World::ApplyTurnEffects() const
	{
		GetWind().ApplyTurnEffects();
		GetCalendar().ApplyTurnEffects();
	}

	int World::GetVersion() const
	{
		return data::game::World::ReadVersion(worldId).value();
	}

	item::Type World::GetCurrencyItemSubtype() const
	{
		return item::Type(data::game::World::ReadCurrencyItemTypeId(worldId).value());
	}

}